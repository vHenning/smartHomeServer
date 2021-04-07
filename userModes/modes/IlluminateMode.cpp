#include "IlluminateMode.h"
#include "../../LED/LEDController.h"

IlluminateMode::IlluminateMode(LEDController *led, const Mode &type)
    : UserMode(type)
    , led(led)
{
    switch (type)
    {
    case eIlluminateBedroomMode:
        devices[eLEDBedroom] = std::make_pair(10, std::bind(&IlluminateMode::turnOnBackgroundLight, this));
        break;
    case eReadingMode:
        devices[eLEDBedroomReading] = std::make_pair(20, std::bind(&IlluminateMode::turnOnReadingLight, this));
        break;
    default:
        break;
    }
}

void IlluminateMode::turnOnBackgroundLight()
{
    led->setFilter(true);
    led->setColor(255, 120, 25);
    led->setDim(1.0);
}

void IlluminateMode::turnOnReadingLight()
{
    led->setFilter(true);
    // Color emulates a spectrum of 3000 Kelvin
    led->setColor(255, 180, 107);
    led->setDim(0.75);
}
