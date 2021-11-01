#include "IlluminateMode.h"
#include "../../LED/LEDManager.h"

IlluminateMode::IlluminateMode() : UserMode(eIlluminateBedroomMode)
{
    devices[eLEDBedroom] = std::make_pair(10, std::bind(&IlluminateMode::turnOnBackgroundLight, this));
}

void IlluminateMode::turnOnBackgroundLight()
{
    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eBedroom);

    led->setFilter(true);
    led->setColor(255, 120, 25);
    led->setDim(1.0);
}
