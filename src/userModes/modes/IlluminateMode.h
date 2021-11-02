#ifndef ILLUMINATEMODE_H
#define ILLUMINATEMODE_H

#include "../UserMode.h"

/**
 * This mode is activated if one or more LEDs shall illuminate an area
 */
class IlluminateMode : public UserMode
{
public:
    IlluminateMode();

private:

    void turnOnBackgroundLight();
    void turnOnTVLight();
};

#endif // ILLUMINATEMODE_H
