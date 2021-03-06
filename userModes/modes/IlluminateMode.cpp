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
        if (getType() == eIlluminateBedroomMode)
        {
            led->setFilter(true);
            led->setColor(253, 150, 50);
            led->setDim(1.0);
        }
        break;
    case eLEDBedroomReading:
        if (getType() == eReadingMode)
        {
            led->setFilter(true);
            led->setColor(253, 150, 50);
            led->setDim(1.0);
        }
        break;
    default:
        break;
    }
}

void IlluminateMode::turnOff(const Device& device)
{
    // TODO: Add off mode and dont turn off here
    switch (device)
    {
    case eLEDBedroom:
        if (getType() == eIlluminateBedroomMode)
        {
            led->setDim(0.0);
        }
        break;
    case eLEDBedroomReading:
        if (getType() == eReadingMode)
        {
            led->setDim(0.0);
        }
        break;
    default:
        break;
    }
}
