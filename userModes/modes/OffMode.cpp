#include "OffMode.h"
#include "../../onkyo/OnkyoServer.h"
#include "../../LED/LEDManager.h"
#include "../../hmi/HMI.h"
#include "../../benq/BenQServer.h"

OffMode::OffMode() : UserMode(eOffMode)
{
    int lowest = std::numeric_limits<int>::lowest();
    devices[eStereo] = lowest;
    devices[eLEDBedroom] = lowest;
    devices[eLEDBedroomReading] = lowest;
    devices[eHMI] = lowest;
    devices[eBeamer] = lowest;
}

void OffMode::turnOn(const Device &device)
{
#ifndef DRYRUN
    switch (device)
    {
    case eStereo:
    {
        OnkyoServer* server = OnkyoServer::getInstance();
        server->setSource(OnkyoServer::eNet);
        server->setVolume(20);
        server->setPower(OnkyoServer::eStandby);
        break;
    }
    case eLEDBedroom:
    {
        LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eBedroom);
        controller->setFilter(true);
        controller->setDim(0.0);
        break;
    }
    case eLEDBedroomReading:
    {
        LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eBedroomReading);
        controller->setFilter(true);
        controller->setDim(0.0);
        break;
    }
    case eHMI:
    {
        HMI* hmi = HMI::getInstance();
        hmi->clearListeners();
        break;
    }
    case eBeamer:
    {
        BenQServer* server = BenQServer::getInstance();
        server->setPowerState(BenQServer::ePowerStandby);
        break;
    }
    }
#endif // DRYRUN
}
