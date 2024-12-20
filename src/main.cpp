#include <boost/bind/bind.hpp>

#include "LED/LEDManager.h"

#include "sensors/PCSensor.h"
#include "sensors/MotionSensor.h"

#include "userModes/ModeManager.h"

#include "hmi/HMI.h"

void init(MotionSensor* motion, ModeManager* manager)
{
    // Add PC handler
    motion->addMotionHandler(2, boost::bind(&ModeManager::addMode, manager, UserMode::ePCMode));
    motion->addMotionStopHandler(2, boost::bind(&ModeManager::removeMode, manager, UserMode::ePCMode), 11);

    // Add motion handler for bedroom
    motion->addMotionHandler(1, boost::bind(&ModeManager::addMode, manager, UserMode::eIlluminateBedroomMode));
    motion->addMotionStopHandler(1, boost::bind(&ModeManager::removeMode, manager, UserMode::eIlluminateBedroomMode), 120);
}

int main (int, char**)
{
    ModeManager* manager = ModeManager::getInstance();
    manager->addMode(UserMode::eOffMode);

    MotionSensor motionSensor;

    init(&motionSensor, manager);

    HMI::getInstance();

//    LEDController* led = LEDManager::getInstance()->getUnit(LEDManager::eDesk);
//    led->setColor(255, 255, 255);
//    led->setDim(1.0);

    while (true)
    {
/*        fprintf(stderr, "Red:\n");
        led->setColor(255, 0, 0);
        usleep(2000000);
	fprintf(stderr, "Green:\n");
	led->setColor(0, 255, 0);
	usleep(2000000);
	led->setColor(0, 0, 255);
	usleep(2000000);
*/
        usleep(60000000);
    }

    return 0;
}
