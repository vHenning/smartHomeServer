#include "LEDManager.h"

LEDManager* LEDManager::getInstance()
{
    static LEDManager manager;
    return &manager;
}

LEDManager::LEDManager()
{
    units[eBedroom] = new LEDController("192.168.0.116");
}

LEDController* LEDManager::getUnit(const Unit &unit)
{
    return units[unit];
}
