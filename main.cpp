#include <boost/bind/bind.hpp>

#include "sensors/PCSensor.h"

#include "userModes/ModeManager.h"

void init(PCSensor* pc, ModeManager* manager)
{
    pc->onHandlerFunctions.push_back(boost::bind(&ModeManager::addMode, manager, UserMode::ePCMode));
    pc->offHandlerFunctions.push_back(boost::bind(&ModeManager::removeMode, manager, UserMode::ePCMode));
}

int main (int, char**)
{
    ModeManager manager;
    manager.addMode(UserMode::eOffMode);

    PCSensor pcSensor;

    init(&pcSensor, &manager);

    while (true)
    {
        usleep(600000000);
    }

    return 0;
}
