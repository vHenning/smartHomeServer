#ifndef TVMODE_H
#define TVMODE_H

#include "../UserMode.h"

class TVMode : public UserMode
{
public:
    TVMode();

    void turnOn();
};

#endif // TVMODE_H