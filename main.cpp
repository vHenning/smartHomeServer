#include <unistd.h>

#include "LED/LEDManager.h"

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
        usleep(200000);
        manager->addMode(UserMode::eIlluminateDiningMode);
    }
    else
    {
        manager->removeMode(UserMode::eIlluminateLivingMode);
        usleep(200000);
        manager->removeMode(UserMode::eIlluminateDiningMode);
    }
}

int main (int, char**)
{
    ModeManager::getInstance()->addMode(UserMode::eOffMode);

    Switch wohnzimmer("schalter_wohnzimmer");
    wohnzimmer.addHandler(std::bind(switchHandler, std::placeholders::_1));

    CECControl::getInstance();

    while (true)
    {
        usleep(1000000);
    }

    return 0;
}
