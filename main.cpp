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
        manager->addMode(UserMode::eCookingMode);
    }
    else
    {
        manager->removeMode(UserMode::eCookingMode);
    }
}

int main (int, char**)
{
    CECControl::getInstance();

    while (true)
    {
        usleep(1000000);
    }

    return 0;
}
