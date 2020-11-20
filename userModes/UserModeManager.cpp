#include "UserModeManager.h"
#include "../hmi/HMI.h"

UserModeManager* UserModeManager::getInstance()
{
    static UserModeManager instance;
    return &instance;
}

void UserModeManager::setMode(UserMode mode)
{
    if (mode == activeMode)
    {
        return;
    }

    if (mode != ePC && pcMode)
    {
        delete pcMode;
        pcMode = 0;
    }

    switch (mode)
    {
    case eDVD:
        if (!dvdMode)
        {
            dvdMode = new DVDMode();
            fprintf(stderr, "Mode set: DVD\n");
        }
        break;
    case ePC:
        if (activeMode == eOff)
        {
            pcMode = new PCMode();
            fprintf(stderr, "Mode set: PC\n");
        }
        break;
    case eOff:
        HMI::getInstance()->clearListeners();
        delete dvdMode;
        dvdMode = 0;
        if (activeMode != eOff && pc->isOn() && !pcMode)
        {
            pcMode = new PCMode();
        }
        fprintf(stderr, "Mode set: off\n");
        break;
    }
    activeMode = mode;
}

void UserModeManager::turnOff(UserMode mode)
{
    if (activeMode != UserModeManager::eOff)
    {
        return;
    }

    switch (mode)
    {
    case ePC:
        if (pcMode)
        {
            fprintf(stderr, "PC Mode turned off\n");
            delete pcMode;
            pcMode = 0;
        }
        break;
    default:
        break;
    }
}

UserModeManager::UserMode UserModeManager::getActiveMode()
{
    return activeMode;
}

UserModeManager::UserModeManager()
{
    pc = new PCSensor();
    pc->handlerFunctions.push_back(std::bind(&UserModeManager::pcStateChanged, this, std::placeholders::_1));
}

void UserModeManager::pcStateChanged(const bool &isOn)
{
    if (isOn && activeMode == eOff)
    {
        setMode(ePC);
    }
    else
    {
        turnOff(ePC);
    }
}
