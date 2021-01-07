#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include "LEDController.h"

class LEDManager
{
public:
    enum Unit
    {
        eBedroom = 0
        , eUnitCount
    };

    static LEDManager* getInstance();

    LEDController* getUnit(const Unit &unit);

private:
    LEDManager();

    LEDController* units[eUnitCount];
};

#endif // LEDMANAGER_H
