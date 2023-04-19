#include "main.h"

#include "Fibre.hpp"
#include "DataAccessor.hpp"


const int TARGET_MOTOR_MAX     = 900;
const int TARGET_MOTOR_MIN     = 100;


class TestMachineFibre : public Fibre
{
public:

    TestMachineFibre(): Fibre("TestMachineFibre")
    {
        FibreManager& thread = FibreManager::getInstance(THREAD_1MS_ID);
        thread.Add(std::shared_ptr<Fibre>(this));
    }

    virtual void Init()
    {
        main_motor_target_.set(0);
    }

    virtual void Run()
    {
        static int time = 0;
        static int tgt = TARGET_MOTOR_MIN;
        static bool direction = false;

        time++;
        if ( time == 10)
        {
            time = 0;
            if (direction == false)
            {
                tgt++;
                if (tgt >= TARGET_MOTOR_MAX)
                {
                    direction = true;
                }
            }
            else
            {
                tgt --;
                if (tgt <= TARGET_MOTOR_MIN)
                {
                    direction = false;
                }
            }
        }
        main_motor_target_.set(tgt);
    }

private:
    DataItem main_motor_target_ {MAIN_MOTOR_TARGET_ID, true};
    DataItem peep_motor_target_ {PEEP_MOTOR_TARGET_ID, true};
    DataItem valve_ie_target_   {VALVE_IE_TARGET_ID, true};
    DataItem pushpull_target_   {PUSHPULL_TARGET_ID, true};
};

static TestMachineFibre testMachineFibre;
