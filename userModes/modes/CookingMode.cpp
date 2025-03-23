#include "CookingMode.h"

#include "../../LED/LEDManager.h"

CookingMode::CookingMode() : UserMode(eCookingMode)
{
    devices[eLEDKitchenSink] = std::make_pair(50, std::bind(&CookingMode::turnOnKitchenLED, this));
}

void CookingMode::turnOnKitchenLED()
{
    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eKitchenSink);
    led->setFilter(true);
    led->setDim(0);
    led->setWhiteTemp(4750);
    led->setWhiteMaxBrightness(true);
    led->setWhiteDim(1);
    led->turnOn(true);
}
