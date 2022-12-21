#include <stdint-gcc.h>

#include "../../app/inc/print_output.hpp"
#include "../../board/st/stm32f302r8_nucleo/Inc/main.h"
#include "../../board/st/stm32f302r8_nucleo/libs/STM32F3xx_HAL_Driver/Inc/stm32f3xx_hal_def.h"
#include "../../board/st/stm32f302r8_nucleo/libs/STM32F3xx_HAL_Driver/Inc/stm32f3xx_hal_gpio.h"
#include "../../board/st/stm32f302r8_nucleo/libs/STM32F3xx_HAL_Driver/Inc/stm32f3xx_hal_i2c.h"
#include "../../infra/inc/crc.hpp"


enum i2c_states{
    I2C_INIT = 0,
    I2C_READ = 1
};
i2c_states i2c_state = I2C_INIT;

uint8_t crc = 0;
uint8_t i2c_retry = 0;
int32_t rawQout = 0;
uint8_t cmd[3] = {0x36, 0x08, 0x00};

void InitQoutSensor()
{
    crc = SF04_CalcCrc (cmd, 2);
    cmd[2] = crc;

    HAL_GPIO_WritePin(CmdQout_GPIO_Port, CmdQout_Pin, GPIO_PIN_SET);
    i2c_state = I2C_INIT;
}

const char* I2C_INIT_ERROR_MSG  = "INIT_ERROR\n\0";
const char* I2C_CRC_ERROR_MSG   = "CHECKSUM_ERROR\n\0";
const char* I2C_READ_ERROR_MSG  = "READING_ERROR\n\0";
const int   I2C_RESET_DURATION  = 300;

void ReadQoutSensor()
{
    static int tick_i2c = 0;
    rawQout = -24576;

    if (i2c_state == I2C_INIT)
    {
        if (tick_i2c < I2C_RESET_DURATION)
        {
            HAL_GPIO_WritePin(CmdQout_GPIO_Port, CmdQout_Pin, GPIO_PIN_SET);
        }
        else if (tick_i2c == I2C_RESET_DURATION)
        {
            HAL_GPIO_WritePin(CmdQout_GPIO_Port, CmdQout_Pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(CmdQout_GPIO_Port, CmdQout_Pin, GPIO_PIN_RESET);
            uint8_t status = HAL_I2C_Master_Transmit(p_hi2c1, SFM3219_ADDRESS<<1, cmd, 3, 1000);
            if (status == HAL_OK)
            {
                i2c_state = I2C_READ;
                i2c_retry = 0;
            }
            else
            {
                printMessage(I2C_INIT_ERROR_MSG);
            }
            tick_i2c = 0;
        }
        tick_i2c++;
    }
    else if (i2c_state == I2C_READ)
    {
        uint8_t i2c_rcv_buff[3] = {0x00, 0x00, 0x00};
        uint8_t status = HAL_I2C_Master_Receive(p_hi2c1, SFM3219_ADDRESS<<1, i2c_rcv_buff, 3, 1000);
        if (status == HAL_OK)
        {
            if (SF04_CheckCrc (i2c_rcv_buff, 2, i2c_rcv_buff[2]) != CHECKSUM_ERROR)
            {
                rawQout = (int16_t)(((uint16_t)i2c_rcv_buff[0])<<8 | i2c_rcv_buff[0]);
            }
            else
            {
                printMessage(I2C_CRC_ERROR_MSG);
                i2c_retry++;
            }
        }
        else
        {
            printMessage(I2C_READ_ERROR_MSG);
            i2c_retry++;
        }
        if (i2c_retry >= 10)
        {
            tick_i2c = 0;
            i2c_state = I2C_INIT;
        }
    }
}


class I2CI
{
public:
	I2CI(uint8_t tx_size_, uint8_t rx_size_)
    {
    }

	~I2CI();

    void Open()
    {
		crc = SF04_CalcCrc (cmd, 2);
		cmd[2] = crc;

		HAL_GPIO_WritePin(CmdQout_GPIO_Port, CmdQout_Pin, GPIO_PIN_SET);
		i2c_state = I2C_INIT;
    }

    void Read();

    void Write();

private:
    uint8_t* tx_buffer_ {};
    uint8_t tx_size_;

    uint8_t* rx_buffer_ {};
	uint8_t rx_size_;
};




