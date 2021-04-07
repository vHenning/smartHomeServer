#ifndef OFFMODE_H
#define OFFMODE_H

#include "../UserMode.h"
#include "../../onkyo/OnkyoServer.h"

class OffMode : public UserMode
{
public:
    OffMode();

private:
    void turnOffStereo();
    void turnOffLEDBedroom();
    void turnOffLEDBedroomReading();
    void turnOffHMI();
    void turnOffBeamer();
};

#endif // OFFMODE_H
