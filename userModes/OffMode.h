#ifndef OFFMODE_H
#define OFFMODE_H

#include "UserMode.h"
#include "../onkyo/OnkyoServer.h"

class OffMode : public UserMode
{
public:
    OffMode();
    virtual void turnOn(const Device &device);
    virtual void turnOff(const Device &device);

protected:
    int previousVolume;
    OnkyoServer::Source previousSource;
    OnkyoServer::PowerState previousPower;
};

#endif // OFFMODE_H
