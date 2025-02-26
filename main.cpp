#include <boost/bind/bind.hpp>

#include "LED/LEDManager.h"

#include "sensors/PCSensor.h"
#include "sensors/MotionSensor.h"

#include "userModes/ModeManager.h"

#include "hmi/HMI.h"

#include "MQTT/devices/Switch.h"

void init(MotionSensor* motion, ModeManager* manager)
{
    // Add PC Handler
    motion->addMotionHandler(2, boost::bind(&ModeManager::addMode, manager, UserMode::ePCMode));
    motion->addMotionStopHandler(2, boost::bind(&ModeManager::removeMode, manager, UserMode::ePCMode), 11);

    // Add motion handler for bedroom
    motion->addMotionHandler(1, boost::bind(&ModeManager::addMode, manager, UserMode::eIlluminateBedroomMode));
    motion->addMotionStopHandler(1, boost::bind(&ModeManager::removeMode, manager, UserMode::eIlluminateBedroomMode), 120);
}

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
    ModeManager* manager = ModeManager::getInstance();
    manager->addMode(UserMode::eOffMode);

    Switch kitchen("unter_schrank_LED");
    kitchen.addHandler(std::bind(switchHandler, std::placeholders::_1));

    MotionSensor motionSensor;
    init(&motionSensor, manager);

    HMI::getInstance();

    while (true)
    {
        usleep(600000000);
    }

    return 0;
}
