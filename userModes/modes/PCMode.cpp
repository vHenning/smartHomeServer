#include "PCMode.h"

#include "../../onkyo/OnkyoServer.h"
#include "../../LED/LEDManager.h"

PCMode::PCMode() : UserMode(ePCMode)
{
    devices[eStereo] = std::make_pair(-5, std::bind(&PCMode::turnOnStereo, this));
    devices[eLEDDesk] = std::make_pair(10, std::bind(&PCMode::turnOnLED, this));
}

void PCMode::turnOnStereo()
{
    OnkyoServer* stereo = OnkyoServer::getInstance();
    stereo->setPower(OnkyoServer::eOn);
    stereo->setSource(OnkyoServer::ePC);
    stereo->setVolume(50);
}

void PCMode::turnOnLED()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eDesk);
    controller->setDim(0.5);
    controller->setWhiteDim(0.5);
    controller->setWhiteTemp(4000);
    controller->setColor(0xFF, 0xFF, 0xFF);
    controller->turnOn(true);
}
