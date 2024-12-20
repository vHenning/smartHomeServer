#ifndef PCMODE_H
#define PCMODE_H

#include "../UserMode.h"

class PCMode : public UserMode
{
public:
    PCMode();

private:
    void turnOnStereo();
    void turnOnLEDDesk();
};

#endif // PCMODE_H
