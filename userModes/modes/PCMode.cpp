#include "PCMode.h"

#include "../../onkyo/OnkyoServer.h"

PCMode::PCMode() : UserMode(ePCMode)
{
    devices[eStereo] = std::make_pair(-5, std::bind(&PCMode::turnOnStereo, this));
}

void PCMode::turnOnStereo()
{
    OnkyoServer* stereo = OnkyoServer::getInstance();
    stereo->setPower(OnkyoServer::eOn);
    stereo->setSource(OnkyoServer::ePC);
    stereo->setVolume(50);
}
