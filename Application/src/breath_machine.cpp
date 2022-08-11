#include "breath_machine.hpp"
#include "trigger.hpp"

#include "Fibre.hpp"
#include "DataAccessor.hpp"

const int TARGET_MOTOR_INSPI     = 750;
const int TARGET_MOTOR_EXPI      = 600;
const int TARGET_MOTOR_PEEP      = 150;
const int TARGET_ON              = 999;
const int TARGET_OFF             = 0;


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
        peep_motor_target_.set(TARGET_MOTOR_PEEP);
        breath_state_.set(state_);
    }

    virtual void Run()
    {
        switch (state_)
        {
        default:
        case EXPI:
            expi_time_.set(time_.value - time_ini_);
            main_motor_target_.set(TARGET_MOTOR_EXPI);
            valve_ie_target_.set(TARGET_OFF);
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
            main_motor_target_.set(TARGET_MOTOR_INSPI);
            valve_ie_target_.set(TARGET_ON);
            if ( trigger_.IsTrigger() )
            {
                inspi_time_.set(0);
                time_ini_ = time_.value;
                state_ = EXPI;
                breath_state_.set(state_);
            }
            break;
        }
    }

private:
    BreathinStates  state_ {EXPI};

    DataItem main_motor_target_ {DataItem(MAIN_MOTOR_TARGET_ID, true)};
    DataItem peep_motor_target_ {DataItem(PEEP_MOTOR_TARGET_ID, true)};
    DataItem valve_ie_target_   {DataItem(VALVE_IE_TARGET_ID, true)};
    DataItem test_target_       {DataItem(TEST_TARGET_ID, true)};

    DataItem breath_state_      {DataItem(BREATH_STATE_ID, true)};
    DataItem inspi_time_        {DataItem(INSPI_TIME_ID, true)};
    DataItem expi_time_         {DataItem(EXPI_TIME_ID, true)};

    Datagram& time_ {DataItem(TIME_ID).get()};
    int32_t time_ini_ {0};

    TriggerManager& trigger_{TriggerManager::GetInstance()};

};

static StateMachineFibre stateMachineFibre;
