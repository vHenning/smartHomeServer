#ifndef USERMODEMANAGER_H
#define USERMODEMANAGER_H

#include "DVDMode.h"
#include "PCMode.h"

#include "../sensors/PCSensor.h"

class UserModeManager
{
public:
    enum UserMode
    {
        eOff
        , ePC
        , ePCVideo
        , eDVD
    };

    static UserModeManager* getInstance();

    void setMode(UserMode mode);
    void turnOff(UserMode mode);

    UserMode getActiveMode();

private:
    UserModeManager();

    void pcStateChanged(const bool &isOn);

    UserMode activeMode;

    PCSensor* pc;

    DVDMode* dvdMode;
    PCMode* pcMode;
};

#endif // USERMODEMANAGER_H
