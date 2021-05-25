#ifndef ILLUMINATEKITCHENMODE_H
#define ILLUMINATEKITCHENMODE_H

#include "../UserMode.h"

class IlluminateKitchenMode : public UserMode
{
public:
    IlluminateKitchenMode();

private:

    void turnOnRGB();
    void turnOnWhite();
};

#endif // ILLUMINATEKITCHENMODE_H
