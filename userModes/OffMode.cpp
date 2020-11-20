#include "OffMode.h"
#include "../onkyo/OnkyoServer.h"

OffMode::OffMode() : UserMode(eOffMode)
  , previousVolume(20)
  , previousSource(OnkyoServer::eNet)
  , previousPower(OnkyoServer::eStandby)
{
    devices[eStereo] = -10;
}

void OffMode::turnOn(const Device &device)
{
    fprintf(stderr, "Turn on Off mode for %s\n", enumToString(device).c_str());
#ifdef DEBUG
#endif

#ifndef DRYRUN

    switch (device)
    {
    case eStereo:
    {
        OnkyoServer *stereo = OnkyoServer::getInstance();

        stereo->setVolume(previousVolume);
        stereo->setSource(previousSource);
        stereo->setPower(previousPower);
        break;
    }
    default:
        break;
    }

#endif
}

void OffMode::turnOff(const Device &device)
{
    fprintf(stderr, "Turn off Off mode for %s\n", enumToString(device).c_str());
#ifdef DEBUG
#endif

#ifndef DRYRUN

    switch (device)
    {
    case eStereo:
    {
        OnkyoServer* stereo = OnkyoServer::getInstance();

        previousVolume = stereo->getVolume();
        previousSource = stereo->getSource();
        previousPower = stereo->getPower();
        break;
    }
    default:
        break;
    }

#endif
}
