#ifndef ONKYOMANAGER_H
#define ONKYOMANAGER_H

#include "OnkyoServer.h"
#include "../sensors/PCSensor.h"

class OnkyoManager
{
public:
    OnkyoManager();

    /**
     * PC
     */
    void PCListener(bool isOn);

    OnkyoServer::PowerState pcPreviousPower;
    OnkyoServer::Source pcPreviousSource;
    int pcPreviousVolume;

    OnkyoServer* device;
};

#endif // ONKYOMANAGER_H
