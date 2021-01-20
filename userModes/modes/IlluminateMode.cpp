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
    default:
        break;
    }
}

void IlluminateMode::turnOn(const Device &device)
{
    switch (device)
    {
    case eLEDBedroom:
        if (type == eIlluminateBedroomMode)
        {
            led->setDim(1.0);
        }
        break;
    default:
        break;
    }
}

void IlluminateMode::turnOff(const Device&)
{
}
