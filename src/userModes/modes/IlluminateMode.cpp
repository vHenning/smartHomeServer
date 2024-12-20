#include "IlluminateMode.h"
#include "../../LED/LEDManager.h"

IlluminateMode::IlluminateMode() : UserMode(eIlluminateBedroomMode)
{
    devices[eLEDBedroom] = std::make_pair(10, std::bind(&IlluminateMode::turnOnBackgroundLight, this));
    devices[eLEDTV] = std::make_pair(10, std::bind(&IlluminateMode::turnOnTVLight, this));
    devices[eLEDDesk] = std::make_pair(5, std::bind(&IlluminateMode::turnOnDeskLight, this));
}

void IlluminateMode::turnOnBackgroundLight()
{
    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eBedroom);

//    led->setFilter(true);
    led->setFilterValues(0.001, 200);
    led->setColor(255, 120, 25);
    led->setDim(1.0);
}

void IlluminateMode::turnOnTVLight()
{
    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eTV);

//    led->setFilter(true);
    led->setFilterValues(0.001, 200);
    led->setColor(255, 120, 25);
    led->setDim(1.0);
}

void IlluminateMode::turnOnDeskLight()
{
    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eDesk);
    led->setColor(255, 120, 25);
    led->setDim(0.5);
}
