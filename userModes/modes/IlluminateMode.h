#ifndef ILLUMINATEMODE_H
#define ILLUMINATEMODE_H

#include "../UserMode.h"
#include "../../LED/LEDController.h"

/**
 * This mode is activated if one or more LEDs shall illuminate an area
 */
class IlluminateMode : public UserMode
{
public:
    IlluminateMode(LEDController* led, const Mode &type);

    virtual void turnOn(const Device &device);
    virtual void turnOff(const Device&);

private:

    LEDController* led;
};

#endif // ILLUMINATEMODE_H
