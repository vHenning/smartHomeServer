#include "PCMode.h"

#include "../../onkyo/OnkyoServer.h"

PCMode::PCMode() : UserMode(ePCMode)
{
    devices[eStereo] = -5;
}

void PCMode::turnOn(const Device &device)
{
    fprintf(stderr, "Turn on PC mode\n");
#ifdef DEBUG
#endif

#ifndef DRYRUN

    switch (device)
    {
    case eStereo:
    {
        OnkyoServer* stereo = OnkyoServer::getInstance();
        stereo->setPower(OnkyoServer::eOn);
        stereo->setSource(OnkyoServer::ePC);
        stereo->setVolume(50);
        break;
    }
    default:
        break;

    }

#endif
}

void PCMode::turnOff(const Device &device)
{
    fprintf(stderr, "Turn off PC mode for device %s\n", enumToString(device).c_str());
#ifdef DEBUG
#endif
}

//PCMode::PCMode()
//{
//#ifdef TEST
//    fprintf(stderr, "PCMode on\n");
//#else
//    OnkyoServer* device = OnkyoServer::getInstance();
//    previousPower = device->getPower();
//    previousSource = device->getSource();
//    previousVolume = device->getVolume();

//    device->setPower(OnkyoServer::eOn);
//    OnkyoServer::Source source = device->setSource(OnkyoServer::ePC);
//    if (source == OnkyoServer::ePC)
//    {
//        device->setVolume(50);
//    }
//#endif
//}

//void PCMode::resetPrevious()
//{
//    previousVolume = 20;
//    previousSource = OnkyoServer::eNet;
//    previousPower = OnkyoServer::eStandby;
//}

//PCMode::~PCMode()
//{
//#ifdef TEST
//    fprintf(stderr, "PCMode off\n");
//#else
//    OnkyoServer* device = OnkyoServer::getInstance();
//    device->setVolume(previousVolume);
//    device->setSource(previousSource);
//    device->setPower(previousPower);
//#endif
//}
