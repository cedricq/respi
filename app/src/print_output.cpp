#include "print_output.hpp"
#include "Fibre.hpp"
#include "DataAccessor.hpp"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <vector>

#define UART_BUF_LEN 255
unsigned char UART3_rxBuffer[UART_BUF_LEN];

DataItemId dataIds[] = {TIME_ID, QOUT_ID, POUT_ID, PPROX_ID, MOTOR_SPEED_ID, MOTOR_CURRENT_ID, MAIN_MOTOR_TARGET_ID, PEEP_MOTOR_TARGET_ID, VALVE_IE_TARGET_ID, TEST_TARGET_ID, BREATH_STATE_ID};

void printMessage(const char* message)
{
    HAL_UART_Transmit(p_huart3, (uint8_t*)message, (uint8_t)(strlen(message)), 100U);
}

void printDatas(const std::vector<Datagram*> datagrams)
{
    char buffer[512]="";
    for(const auto& data: datagrams)
    {
        char txt[64];
        sprintf(txt, "%d ", static_cast<int>(data->value));
        strcat(buffer, txt);
    }
    strcat(buffer, "\n\0");
    HAL_UART_Transmit_DMA(p_huart3, (uint8_t*)buffer, (uint8_t)(strlen(buffer)));
}

void printTelePlot(const std::vector<Datagram*> datagrams)
{
    char buffer[512]="";
    for(const auto& data: datagrams)
    {
        int val = static_cast<int>(data->value);
        int div = static_cast<int>(data->div);
        char txt[64];
        if ( div > 1)
        {
            sprintf(txt, ">%s:%d.%3d", data->name, val / div, val % div);
        }
        else
        {
            sprintf(txt, ">%s:%d", data->name, val);
        }
        strcat(buffer, txt);
        strcat(buffer, "\r\n");
    }
    strcat(buffer, "\0");
    HAL_UART_Transmit_DMA(p_huart3, (uint8_t*)buffer, (uint8_t)(strlen(buffer)));
}


class PrintFibre : public Fibre
{
public:
    PrintFibre(): Fibre("PrintFibre")
    {
        FibreManager& mgr = FibreManager::getInstance(THREAD_POLLED_ID);
        mgr.Add(std::shared_ptr<Fibre>(this));
    }

    virtual void Init()
    {
        for (uint32_t i = 0; i < sizeof(dataIds)/sizeof(dataIds[0]); i++)
        {
            datagrams_.push_back(&(DataItem(dataIds[i]).get()));
        }

        HAL_UART_Receive_IT(p_huart3, UART3_rxBuffer, 1);
    }

    virtual void Run()
    {
        static DataItem time(TIME_ID);
        if (time.get().value%10 == 0)
        {
            printTelePlot(datagrams_);
            //printDatas(datagrams_);
        }
    }

    std::vector<Datagram*> datagrams_ {};
};

static PrintFibre printFibre;

