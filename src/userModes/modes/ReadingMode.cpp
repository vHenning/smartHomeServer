#include "ReadingMode.h"

#include "../../LED/LEDManager.h"

ReadingMode::ReadingMode() : UserMode(eReadingMode)
{
    devices[eLEDBedroomReading] = std::make_pair(20, std::bind(&ReadingMode::turnOnReadingLight, this));
}

void ReadingMode::turnOnReadingLight()
{
    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eBedroomReading);

    led->setFilter(true);
    // Color emulates a spectrum of 3000 Kelvin
    led->setColor(255, 180, 107);
    led->setDim(0.75);
}
