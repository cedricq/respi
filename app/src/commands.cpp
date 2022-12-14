#include "Fibre.hpp"
#include "DataAccessor.hpp"

#include "main.h"

#include "stm32f3xx_hal.h"

struct PWM
{
    volatile uint32_t& pwm;
    uint32_t period;
};

const PWM pwm1 = {.pwm = TIM15->CCR1, .period = TIM15_PERIOD_TICKS};
const PWM pwm2 = {.pwm = TIM15->CCR2, .period = TIM15_PERIOD_TICKS};
const PWM pwm3 = {.pwm = TIM2->CCR1,  .period = TIM2_PERIOD_TICKS};     // PA5
const PWM pwm4 = {.pwm = TIM2->CCR3,  .period = TIM2_PERIOD_TICKS};     // PB10

int32_t TargetToDAC(int32_t target, int32_t div)
{
    static const int32_t MAX_RAW_DAC = 4095;
    return target * MAX_RAW_DAC / (100*div);
}

void UpdatePWM(const PWM& pwm, int32_t target, int32_t div)
{
    pwm.pwm = (target * pwm.period) / (100 * div);
}

const int CMD_MOTOR_INSPI     = 2500;
const int CMD_MOTOR_EXPI      = 500;
const int INSPI_TIME          = 1000;
const int EXPI_TIME           = 2000;

class CommandFibre : public Fibre
{
public:
    CommandFibre(): Fibre("CommandFibre")
    {
        FibreManager& thread = FibreManager::getInstance(THREAD_1MS_ID);
        thread.Add(std::shared_ptr<Fibre>(this));
    }

    virtual void Init()
    {
        UpdatePWM(pwm1, 0, 1);
        UpdatePWM(pwm2, 0, 1);
        TIM15->CCER |= TIM_CCER_CC1E;
        TIM15->CCER |= TIM_CCER_CC2E;
        HAL_TIMEx_PWMN_Start(p_htim15, HAL_TIM_ACTIVE_CHANNEL_1);

        UpdatePWM(pwm3, 0, 1);
        UpdatePWM(pwm4, 0, 1);
        TIM2->CCER |= TIM_CCER_CC1E;
        TIM2->CCER |= TIM_CCER_CC3E;
        HAL_TIMEx_PWMN_Start(p_htim2, HAL_TIM_ACTIVE_CHANNEL_1);
    }

    virtual void Run()
    {
        static int cnt = 0;
        static int val = 0;
        static int sens = 0;
        static DataItem main_motor_target(MAIN_MOTOR_TARGET_ID);
        static DataItem peep_motor_target(PEEP_MOTOR_TARGET_ID);
        static DataItem valve_ie_target(VALVE_IE_TARGET_ID);
        static DataItem test_target(TEST_TARGET_ID);

        DAC1->DHR12R1 = TargetToDAC(main_motor_target.get().value, main_motor_target.get().div);
        UpdatePWM(pwm1, valve_ie_target.get().value, valve_ie_target.get().div);
        UpdatePWM(pwm2, peep_motor_target.get().value, peep_motor_target.get().div);
        UpdatePWM(pwm3, test_target.get().value, test_target.get().div);
        //UpdatePWM(pwm4, test_target.get().value, test_target.get().div);

        cnt++;
        if (cnt > 2)
        {
            if (sens == 0)
            {
                val++;
                if (val >= 999)
                {
                    sens = 1;
                    HAL_GPIO_WritePin(IN1_ACT_GPIO_Port, IN1_ACT_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(IN2_ACT_GPIO_Port, IN2_ACT_Pin, GPIO_PIN_SET);
                    UpdatePWM(pwm4, 900, 10);
                }
            }
            else
            {
                val--;
                if (val <= 0)
                {
                    sens = 0;
                    HAL_GPIO_WritePin(IN2_ACT_GPIO_Port, IN2_ACT_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(IN1_ACT_GPIO_Port, IN1_ACT_Pin, GPIO_PIN_SET);
                    UpdatePWM(pwm4, 500, 10);
                }
            }
            cnt = 0;
        }

    }
};

static CommandFibre commandFibre;



