#include "LEDManager.h"

LEDManager* LEDManager::getInstance()
{
    static LEDManager manager;
    return &manager;
}

LEDManager::LEDManager()
{
    units[eBedroom] = new LEDController("192.168.0.117", 0);
    units[eBedroomReading] = new LEDController(1, units[eBedroom]);
}

LEDController* LEDManager::getUnit(const Unit &unit)
{
    return units[unit];
}
