#ifndef PCMODE_H
#define PCMODE_H

#include "../UserMode.h"

class PCMode : public UserMode
{
public:
    PCMode();

private:
    void turnOnStereo();
    void turnOnLED();
};

#endif // PCMODE_H
