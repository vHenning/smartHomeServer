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
    devices[eLEDTV] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDTV, this));
    devices[eHMI] = std::make_pair(lowest, std::bind(&OffMode::turnOffHMI, this));
    devices[eKodi] = std::make_pair(lowest, std::bind(&OffMode::turnOffKodi, this));
    devices[eLEDDesk] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDDesk, this));
    devices[eLEDKitchenSink] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDKitchen, this));
    devices[eLEDDining] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDDining, this));
    devices[eLEDLiving] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDLiving, this));
    devices[eLEDLivingTV] = std::make_pair(lowest, std::bind(&OffMode::turnOffLEDLivingTV, this));
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

void OffMode::turnOffLEDTV()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eTV);
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

void OffMode::turnOffLEDDesk()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eDesk);
    controller->turnOn(false);
}

void OffMode::turnOffLEDKitchen()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eKitchenSink);
    controller->turnOn(false);
}

void OffMode::turnOffLEDDining()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eDiningRoom);
    controller->turnOn(false);
}
void OffMode::turnOffLEDLiving()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eLivingRoom);
    controller->turnOn(false);
}
void OffMode::turnOffLEDLivingTV()
{
    LEDController* controller = LEDManager::getInstance()->getUnit(LEDManager::eLivingRoomTV);
    controller->turnOn(false);
}
