#include "Fibre.hpp"
#include "main.h"

#include "crc.hpp"
#include "DataAccessor.hpp"
#include "print_output.hpp"

#include "stm32f3xx_hal.h"

#include <string.h>

#define ADC_IN1_PA0_POUT     0
#define ADC_IN11_PB0_PROX    1
#define ADC_IN6_PC0_I_MOT    2
#define ADC_IN7_PC1_S_MOT    3
#define ADC_BUF_LEN         (ADC_IN7_PC1_S_MOT+1)
uint32_t adc_buf[ADC_BUF_LEN];


#define SFM3219_ADDRESS     0x2E
#define SFM3219_START_AIR   0x3608

const int32_t offsetQout = -57;

const int32_t offsetPout = -366;
const int32_t offsetPprox = -366;

const int32_t MAX_CURRENT   = 300;
const int32_t MAX_SPEED     = 60000;
const int32_t MAX_RAW_ADC   = 4095;

int32_t VoltageTo01mbar(int32_t volt, int32_t offset)
{
    return ( ( ( ( volt - 33 ) * 13790 ) / 264 ) - 6895) - offset;
}

int32_t RawADCToVolt(int32_t raw)
{
    return raw * 330 / MAX_RAW_ADC;
}

int32_t RawSFM3019ToLmin(int32_t raw, int32_t offset)
{
    return ( ( 100 * ( raw + 24576 ) ) / 170 ) - offset;
}

int32_t RawToCal(int32_t raw, int32_t max_cal, int32_t max_raw)
{
    return (raw * max_cal) / max_raw;
}

void InitADC()
{
    HAL_ADC_Start_DMA(p_hadc1, (uint32_t*)adc_buf, ADC_BUF_LEN);
}

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

void ReadQoutSensor()
{
    static int tick_i2c = 0;
    rawQout = -24576;

    if (i2c_state == I2C_INIT)
    {
        if (tick_i2c <= 300)
        {
            HAL_GPIO_WritePin(CmdQout_GPIO_Port, CmdQout_Pin, GPIO_PIN_SET);
        }
        else if (tick_i2c <= 400)
        {
            HAL_GPIO_WritePin(CmdQout_GPIO_Port, CmdQout_Pin, GPIO_PIN_RESET);
        }
        else
        {
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


void UpdateMeasurements()
{
    static DataItem qout(QOUT_ID, true);
    static DataItem pout(POUT_ID, true);
    static DataItem pprox(PPROX_ID, true);
    static DataItem motor_current(MOTOR_CURRENT_ID, true);
    static DataItem motor_speed(MOTOR_SPEED_ID, true);

    qout.set(RawSFM3019ToLmin(rawQout, offsetQout));
    pout.set(VoltageTo01mbar( RawADCToVolt( adc_buf[ADC_IN1_PA0_POUT] ), offsetPout ));
    pprox.set(VoltageTo01mbar( RawADCToVolt( adc_buf[ADC_IN11_PB0_PROX] ), offsetPprox ));
    motor_current.set(RawToCal(adc_buf[ADC_IN6_PC0_I_MOT], MAX_CURRENT, MAX_RAW_ADC));
    motor_speed.set(RawToCal(adc_buf[ADC_IN7_PC1_S_MOT], MAX_SPEED, MAX_RAW_ADC));
}


class MeasurementFibre : public Fibre
{
public:
    MeasurementFibre(): Fibre("MeasurementFibre")
    {
        FibreManager& thread = FibreManager::getInstance(THREAD_1MS_ID);
        thread.Add(std::shared_ptr<Fibre>(this));
    }

    virtual void Init()
    {
        InitADC();
        InitQoutSensor();
    }
    virtual void Run()
    {
        static DataItem time(TIME_ID, true);
        time.set(time.get().value + 1 );

        ReadQoutSensor();
        UpdateMeasurements();
    }
};

static MeasurementFibre measurementFibre;



