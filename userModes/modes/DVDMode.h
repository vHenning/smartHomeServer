#ifndef DVD_MODE_H
#define DVD_MODE_H

#include "../../benq/BenQServer.h"
#include "../../onkyo/OnkyoServer.h"
#include "../../video/VideoPlayer.h"

class DVDMode
{
public:
    DVDMode();
    ~DVDMode();

    OnkyoServer::PowerState previousPower;
    OnkyoServer::Source previousSource;
    int previousVolume;

    VideoPlayer* player;
};

#endif // DVD_MODE_H
