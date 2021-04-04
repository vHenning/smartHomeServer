#include "IlluminateMode.h"
#include "../../LED/LEDController.h"

IlluminateMode::IlluminateMode(LEDController *led, const Mode &type)
    : UserMode(type)
    , led(led)
{
    switch (type)
    {
    case eIlluminateBedroomMode:
        devices[eLEDBedroom] = 10;
        break;
    case eReadingMode:
        devices[eLEDBedroomReading] = 20;
        break;
    default:
        break;
    }
}

void IlluminateMode::turnOn(const Device &device)
{
    switch (device)
    {
    case eLEDBedroom:
        if (getType() == eIlluminateBedroomMode)
        {
            led->setFilter(true);
            led->setColor(255, 120, 25);
            led->setDim(1.0);
        }
        break;
    case eLEDBedroomReading:
        if (getType() == eReadingMode)
        {
            led->setFilter(true);
            // Color emulates a spectrum of 3000 Kelvin
            led->setColor(255, 180, 107);
            led->setDim(0.75);
        }
        break;
    default:
        break;
    }
}
