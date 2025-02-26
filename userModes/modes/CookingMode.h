#ifndef COOKING_MODE_H
#define COOKING_MODE_H

#include "../UserMode.h"

class CookingMode : public UserMode
{
public:
    CookingMode();

private:

    void turnOnKitchenLED();
};

#endif // COOKING_MODE_H