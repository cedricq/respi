#include "print_output.hpp"
#include "Fibre.hpp"
#include "DataAccessor.hpp"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#define UART_TX_BUF_LEN 256
#define UART_RX_BUF_LEN 10
char UART3_txBuffer[UART_TX_BUF_LEN];
char UART3_rxBuffer[UART_RX_BUF_LEN];
unsigned int index_received = 0;

#define RX_BUF_SIZE 32
char RXBuffer[RX_BUF_SIZE];
unsigned int index_rx = 0;

DataItemId dataIds[] = {QOUT_ID, POUT_ID, PPROX_ID, MOTOR_SPEED_ID, MOTOR_CURRENT_ID, MAIN_MOTOR_TARGET_ID, PEEP_MOTOR_TARGET_ID, VALVE_IE_TARGET_ID, PUSHPULL_TARGET_ID, BREATH_STATE_ID};


void newCharRxUART3(void)
{
	char newChar = UART3_rxBuffer[index_received];
	RXBuffer[index_rx] = newChar;
	index_rx++;
	if ( index_rx >= RX_BUF_SIZE ) index_rx = 0;
	RXBuffer[index_rx] = '\0';

	UART3_rxBuffer[index_received + 1] = '\0';
	if ( strchr(UART3_rxBuffer, '\n') != NULL)
	{
		if (strchr(UART3_rxBuffer, ':') != NULL)
		{
			char * stop;
			int di = strtol((UART3_rxBuffer), &stop, 10);
			int index = stop - UART3_rxBuffer;
			index++;
			int val = strtol((UART3_rxBuffer+index), &stop, 10);

			DataItem DI(di, true);
			DI.setOverride(val);
		}
		UART3_rxBuffer[0] = '\0';
		index_received = 0;
	}
	else
	{
		index_received++;
		index_received = index_received % UART_RX_BUF_LEN;
	}
}

void waitForNewCharRxUART3(void)
{
    HAL_UART_Receive_IT(p_huart3, (UART3_rxBuffer+index_received), 1);
}

void printMessage(const char* message)
{
    HAL_UART_Transmit(p_huart3, (uint8_t*)message, (uint8_t)(strlen(message)), 100U);
}

void printDatas(const std::vector<Datagram*> datagrams)
{
    UART3_txBuffer[0] = '\0';

    for(const auto& data: datagrams)
    {
        char txt[64];
        sprintf(txt, "%d ", static_cast<int>(data->value));
        strcat(UART3_txBuffer, txt);
    }
    strcat(UART3_txBuffer, "\n\0");

    HAL_UART_Transmit_DMA(p_huart3, (uint8_t*)UART3_txBuffer, (uint8_t)(strlen(UART3_txBuffer)));
}

void printTelePlot(const std::vector<Datagram*> datagrams)
{
    UART3_txBuffer[0] = '\0';

    for(const auto& data: datagrams)
    {
        char txt[32];
        sprintf(txt, ">%s:%f\n", data->name, static_cast<float>(data->value)/data->div);
        strcat(UART3_txBuffer, txt);
    }
    strcat(UART3_txBuffer, "\0");
    HAL_UART_Transmit_DMA(p_huart3, (uint8_t*)UART3_txBuffer, (uint8_t)(strlen(UART3_txBuffer)));
}

class PrintFibre : public Fibre
{
public:
    PrintFibre(): Fibre("PrintFibre")
    {
        FibreManager& mgr = FibreManager::getInstance(THREAD_10MS_ID);
        mgr.Add(std::shared_ptr<Fibre>(this));
    }

    virtual void Init()
    {
        for (uint32_t i = 0; i < sizeof(dataIds)/sizeof(dataIds[0]); i++)
        {
            datagrams_.push_back(&(DataItem(dataIds[i]).get()));
        }
        HAL_UART_Receive_IT(p_huart3, (UART3_rxBuffer+index_received), 1);
    }

    virtual void Run()
    {
        static DataItem stream(STREAM_ON_ID);
        if (true)
        //if(stream.get().value)
        {
        	printTelePlot(datagrams_);
        	//printDatas(datagrams_);
        }
    }

private:
    std::vector<Datagram*> datagrams_ {};
};

static PrintFibre printFibre;

class CommandsFibre : public Fibre
{
public:
	CommandsFibre(): Fibre("CommandsFibre")
    {
        FibreManager& mgr = FibreManager::getInstance(THREAD_POLLED_ID);
        mgr.Add(std::shared_ptr<Fibre>(this));
    }

    virtual void Init()
    {
    }

    virtual void Run()
    {
    	if( index_rx > 0 )
    	{
    		char* token = strstr(RXBuffer, "\n");
    		if (token != NULL)
    		{
    			HAL_UART_Transmit_DMA(p_huart3, (uint8_t*)RXBuffer, (uint8_t)(token - RXBuffer + 1));
    			char line[256];
    			strcpy(line, RXBuffer);
    			char* c = strstr(line, "\n");
    			c = '\0';
    			//CommandsManger -> line
    			index_rx = 0;
    		}
    	}
    }

};

//static CommandsFibre commandsFibre;
