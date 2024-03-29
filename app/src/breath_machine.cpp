#include "breath_machine.hpp"
#include "trigger.hpp"

#include "main.h"

#include "Fibre.hpp"
#include "DataAccessor.hpp"

const int TARGET_QOUT_INSPI     = 3000;
const int TARGET_QOUT_EXPI      = 600;
const int TARGET_MOT_INSPI      = 500;
const int TARGET_MOT_EXPI       = 100;

const int TARGET_MOTOR_PEEP_INSPI = 300;
const int TARGET_MOTOR_PEEP_EXPI  = 150;

const int TARGET_ON              = 999;
const int TARGET_OFF             = 0;

const int PUSH_FORCE             = 900;
const int PULL_FORCE             = 500;


class StateMachineFibre : public Fibre
{
public:

    StateMachineFibre(): Fibre("StateMachineFibre")
    {
        FibreManager& thread = FibreManager::getInstance(THREAD_1MS_ID);
        thread.Add(std::shared_ptr<Fibre>(this));
    }

    virtual void Init()
    {
        time_ini_ = time_.value;
        breath_state_.set(state_);

        main_motor_target_.set(TARGET_MOT_EXPI);
    }

    virtual void Run()
    {
        switch (state_)
        {
        default:
        case EXPI:
            expi_time_.set(time_.value - time_ini_);
            peep_motor_target_.set(TARGET_MOTOR_PEEP_EXPI);
            target_qout_.set(TARGET_QOUT_EXPI);
            valve_ie_target_.set(TARGET_OFF);

            main_motor_target_.set(TARGET_MOT_EXPI);

            // Push actuonix
            HAL_GPIO_WritePin(IN2_ACT_GPIO_Port, IN2_ACT_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN1_ACT_GPIO_Port, IN1_ACT_Pin, GPIO_PIN_SET);
            pushpull_target_.set(PULL_FORCE);

            if ( trigger_.IsTrigger() )
            {
                expi_time_.set(0);
                time_ini_ = time_.value;
                state_ = INSPI;
                breath_state_.set(state_);
            }
            break;
        case INSPI:
            inspi_time_.set(time_.value - time_ini_);

            main_motor_target_.set(TARGET_MOT_INSPI);

            peep_motor_target_.set(TARGET_MOTOR_PEEP_INSPI);
            target_qout_.set(TARGET_QOUT_INSPI);
            valve_ie_target_.set(TARGET_ON);

            // Pull actuonix
            HAL_GPIO_WritePin(IN1_ACT_GPIO_Port, IN1_ACT_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN2_ACT_GPIO_Port, IN2_ACT_Pin, GPIO_PIN_SET);
            pushpull_target_.set(PUSH_FORCE);

            if ( trigger_.IsTrigger() )
            {
                inspi_time_.set(0);
                time_ini_ = time_.value;
                state_ = EXPI;
                breath_state_.set(state_);
            }
            break;
        }


        //int32_t tgt = (target_qout_.get().value * 18) / 1000 + 27;
        //main_motor_target_.set(tgt);

    }

private:
    BreathinStates  state_ {EXPI};

    DataItem target_qout_ {QOUT_TARGET_ID, true};
    DataItem target_pout_ {POUT_TARGET_ID, true};

    DataItem main_motor_target_ {MAIN_MOTOR_TARGET_ID, true};
    DataItem peep_motor_target_ {PEEP_MOTOR_TARGET_ID, true};
    DataItem valve_ie_target_   {VALVE_IE_TARGET_ID, true};
    DataItem pushpull_target_   {PUSHPULL_TARGET_ID, true};

    DataItem breath_state_      {BREATH_STATE_ID, true};
    DataItem inspi_time_        {INSPI_TIME_ID, true};
    DataItem expi_time_         {EXPI_TIME_ID, true};

    Datagram& time_ {DataItem(TIME_ID).get()};
    int32_t time_ini_ {0};

    TriggerManager& trigger_{TriggerManager::GetInstance()};

};

static StateMachineFibre stateMachineFibre;
