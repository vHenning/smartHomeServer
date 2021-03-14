#include <boost/bind/bind.hpp>

#include "LED/LEDManager.h"

#include "sensors/PCSensor.h"
#include "sensors/MotionSensor.h"

#include "userModes/ModeManager.h"

void init(PCSensor* pc, MotionSensor* motion, ModeManager* manager)
{
    // Add PC handler
    pc->onHandlerFunctions.push_back(boost::bind(&ModeManager::addMode, manager, UserMode::ePCMode));
    pc->offHandlerFunctions.push_back(boost::bind(&ModeManager::removeMode, manager, UserMode::ePCMode));

    // Add motion handler for bedroom
    motion->addMotionHandler(1, boost::bind(&ModeManager::addMode, manager, UserMode::eIlluminateBedroomMode));
    motion->addMotionStopHandler(1, boost::bind(&ModeManager::removeMode, manager, UserMode::eIlluminateBedroomMode), 120);
}

int main (int, char**)
{
    ModeManager manager = *ModeManager::getInstance();
    manager.addMode(UserMode::eOffMode);

    PCSensor pcSensor;
    MotionSensor motionSensor;

    init(&pcSensor, &motionSensor, &manager);

    while (true)
    {
        usleep(600000000);
    }

    return 0;
}
