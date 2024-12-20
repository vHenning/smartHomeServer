#include "PCMode.h"

#include "../../onkyo/OnkyoServer.h"
#include "../../LED/LEDManager.h"

PCMode::PCMode() : UserMode(ePCMode)
{
    devices[eStereo] = std::make_pair(-5, std::bind(&PCMode::turnOnStereo, this));
    devices[eLEDDesk] = std::make_pair(10, std::bind(&PCMode::turnOnLEDDesk, this));
}

void PCMode::turnOnStereo()
{
    OnkyoServer* stereo = OnkyoServer::getInstance();
    stereo->setPower(OnkyoServer::eOn);
    stereo->setSource(OnkyoServer::ePC);
    stereo->setVolume(50);
}

void PCMode::turnOnLEDDesk()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eDesk);
    controller->setColor(255, 120, 25);
    controller->setDim(0.5);
}
