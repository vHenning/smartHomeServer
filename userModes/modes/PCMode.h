#ifndef PCMODE_H
#define PCMODE_H

#include "../UserMode.h"

class PCMode : public UserMode
{
public:
    PCMode();

    virtual void turnOn(const Device &device);
};

#endif // PCMODE_H
