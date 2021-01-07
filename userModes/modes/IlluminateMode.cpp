#include "IlluminateMode.h"
#include "../../LED/LEDController.h"

IlluminateMode::IlluminateMode(LEDController *led, const Mode &type)
    : UserMode(type)
    , led(led)
{

}

void IlluminateMode::turnOn(const Device &device)
{
    switch (device)
    {
    case eLEDBedroom:
        led->setDim(1.0);
        break;
    default:
        break;
    }
}

void IlluminateMode::turnOff(const Device&)
{
}
