#include <unistd.h>

#include "LED/LEDManager.h"
#include "LED/Dimmer.h"

#include "sensors/PCSensor.h"
#include "sensors/MotionSensor.h"

#include "userModes/ModeManager.h"

#include "hmi/HMI.h"

#include "MQTT/devices/Switch.h"

#include "CEC/CECControl.h"

void switchHandler(bool on)
{
    ModeManager* manager = ModeManager::getInstance();
    if (on)
    {
        manager->addMode(UserMode::eIlluminateLivingMode);
        manager->addMode(UserMode::eIlluminateDiningMode);
    }
    else
    {
        manager->removeMode(UserMode::eIlluminateLivingMode);
        manager->removeMode(UserMode::eIlluminateDiningMode);
    }
}

void dimHandler(double brightness)
{
    ModeManager* manager = ModeManager::getInstance();
    if (manager->getActiveMode(UserMode::eLEDDining) == UserMode::eIlluminateDiningMode)
    {
        LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eDiningRoom);
        led->setFilter(true);
        led->setDim(brightness);
        led->setWhiteDim(brightness);
    }
    if (manager->getActiveMode(UserMode::eLEDLivingTV) == UserMode::eIlluminateLivingMode)
    {
        LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eLivingRoomTV);
        led->setFilter(true);
        led->setDim(brightness);
        led->setWhiteDim(brightness);
    }
    if (manager->getActiveMode(UserMode::eLEDLiving) == UserMode::eIlluminateLivingMode)
    {
        LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eLivingRoom);
        led->setFilter(true);
        led->setDim(brightness);
        led->setWhiteDim(brightness);
    }
}

int main (int, char**)
{
    ModeManager::getInstance()->addMode(UserMode::eOffMode);

    Dimmer dimmer(0.45);
    Switch wohnzimmer("schalter_wohnzimmer");
    wohnzimmer.addHandler(std::bind(&Dimmer::switchChanged, &dimmer, std::placeholders::_1));

    dimmer.setDimListener(std::bind(&dimHandler, std::placeholders::_1));
    dimmer.setOnOffListener(std::bind(&switchHandler, std::placeholders::_1));

    CECControl::getInstance();

    while (true)
    {
        usleep(1000000);
    }

    return 0;
}
