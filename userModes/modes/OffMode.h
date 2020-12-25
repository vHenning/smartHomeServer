#ifndef OFFMODE_H
#define OFFMODE_H

#include "../UserMode.h"
#include "../../onkyo/OnkyoServer.h"

// TODO: OffMode becomes onkyo music mode triggered by OnkyoServer receiving mode change to net or phono or bluetooth

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
