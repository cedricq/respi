#include "print_output.hpp"
#include "Fibre.hpp"
#include "DataAccessor.hpp"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <vector>

#define UART_BUF_LEN 256
char UART3_txBuffer[UART_BUF_LEN];

DataItemId dataIds[] = {QOUT_ID, POUT_ID, PPROX_ID, MOTOR_SPEED_ID, MOTOR_CURRENT_ID, MAIN_MOTOR_TARGET_ID, PEEP_MOTOR_TARGET_ID, VALVE_IE_TARGET_ID, PUSHPULL_TARGET_ID, BREATH_STATE_ID};

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
        sprintf(txt, ">%s:%d\n", data->name, static_cast<int>(data->value));
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
        //HAL_UART_Receive_IT(p_huart3, UART3_rxBuffer, 1);
    }

    virtual void Run()
    {
        printTelePlot(datagrams_);
        //printDatas(datagrams_);
    }

    std::vector<Datagram*> datagrams_ {};
};

static PrintFibre printFibre;

