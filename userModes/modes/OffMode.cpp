#include "OffMode.h"
#include "../../onkyo/OnkyoServer.h"
#include "../../LED/LEDManager.h"
#include "../../hmi/HMI.h"
#include "../../benq/BenQServer.h"
#include "../../video/KodiController.h"

OffMode::OffMode() : UserMode(eOffMode)
{
    int lowest = std::numeric_limits<int>::lowest();
    devices[eStereo] = std::make_pair(lowest, std::bind(&OffMode::turnOffStereo, this));
    devices[eLEDBedroom] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDBedroom, this));
    devices[eLEDBedroomReading] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDBedroomReading, this));
    devices[eLEDKitchenRGB] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDKitchenRGB, this));
    devices[eLEDKitchenWhite] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDKitchenWhite, this));
    devices[eHMI] = std::make_pair(lowest, std::bind(&OffMode::turnOffHMI, this));
    devices[eBeamer] = std::make_pair(lowest, std::bind(&OffMode::turnOffBeamer, this));
    devices[eKodi] = std::make_pair(lowest, std::bind(&OffMode::turnOffKodi, this));
}

void OffMode::turnOffBeamer()
{
    BenQServer* server = BenQServer::getInstance();
    server->setPowerState(BenQServer::ePowerStandby);
}

void OffMode::turnOffHMI()
{
    HMI* hmi = HMI::getInstance();
    hmi->clearListeners();
}

void OffMode::turnOffLEDBedroom()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eBedroom);
    controller->setFilter(true);
    controller->setDim(0.0);
}

void OffMode::turnOffLEDBedroomReading()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eBedroomReading);
    controller->setFilter(true);
    controller->setDim(0.0);
}

void OffMode::turnOffLEDKitchenRGB()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eKitchenRGB);
    controller->setFilter(true);
    controller->setDim(0.0);
}

void OffMode::turnOffLEDKitchenWhite()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eKitchenW);
    controller->setFilter(true);
    controller->setDim(0.0);
}

void OffMode::turnOffStereo()
{
    OnkyoServer* server = OnkyoServer::getInstance();
    server->setSource(OnkyoServer::eNet);
    server->setVolume(20);
    server->setPower(OnkyoServer::eStandby);
}

void OffMode::turnOffKodi()
{
    KodiController::getInstance()->quit();
}
