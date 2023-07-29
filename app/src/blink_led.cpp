#include "Fibre.hpp"
#include "main.h"

#include "stm32f3xx_hal.h"

class BlinkFibre : public Fibre
{
public:
    BlinkFibre(): Fibre("BlinkFibre")
    {
        FibreManager& thread = FibreManager::getInstance(THREAD_1S_ID);
        thread.Add(std::shared_ptr<Fibre>(this));
    }

    virtual void Init()
    {
    	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }

    virtual void Run()
    {
    	static int pin = GPIO_PIN_RESET;
    	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, pin);
		pin = (pin+1) %2;
    }
};

static BlinkFibre blinkFibre;



