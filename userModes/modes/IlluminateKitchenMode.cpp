#include "IlluminateKitchenMode.h"

#include "../../LED/LEDManager.h"

IlluminateKitchenMode::IlluminateKitchenMode() : UserMode(eIlluminateKitchenMode)
{
    devices[eLEDKitchenRGB] = std::make_pair(20, std::bind(&IlluminateKitchenMode::turnOnRGB, this));
    devices[eLEDKitchenWhite] = std::make_pair(20, std::bind(&IlluminateKitchenMode::turnOnWhite, this));
}

void IlluminateKitchenMode::turnOnRGB()
{
    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eKitchenRGB);
    led->setFilter(true);
    // Color emulates a spectrum of 3000 Kelvin
    led->setColor(255, 180, 107);
    led->setDim(1.0);
}

void IlluminateKitchenMode::turnOnWhite()
{
    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eKitchenW);
    led->setFilter(true);
    led->setColor(255, 255, 255);
    led->setDim(1.0);
}
