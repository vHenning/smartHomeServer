#ifndef ILLUMINATE_ASCHEBERG_H
#define ILLUMINATE_ASCHEBERG_H

#include "../UserMode.h"
#include "../../LED/LEDController.h"

class IlluminateAscheberg : public UserMode
{
public:
    IlluminateAscheberg(Mode mode);

    void turnOn(LEDController* led);
};

#endif // ILLUMINATE_ASCHEBERG_H