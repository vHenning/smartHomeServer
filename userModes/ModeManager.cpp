#include "modes/DVDMode.h"
#include "ModeManager.h"
#include "UserMode.h"
#include "modes/OffMode.h"
#include "modes/PCMode.h"

ModeManager::ModeManager()
{
    // add off mode
}

void ModeManager::addMode(UserMode::Mode toAdd)
{
    UserMode* mode = getUserMode(toAdd);

    // Check if mode already exists
    // TODO if mode already exists, push it in front in its same dominance group: remove and add again, then check if it is first

    if (isActive(mode->getType()))
    {
        fprintf(stderr, "User Mode is already active\n");
    }

    std::vector<UserMode::Device> newDevices = mode->getDevices();

    for (size_t i = 0; i < newDevices.size(); ++i)
    {
        devices[newDevices[i]].insert(std::pair<int, UserMode*>(mode->getDominance(newDevices[i]), mode));

        std::multimap<int, UserMode*>::const_iterator last = devices[newDevices[i]].end();
        --last;

        if (last->second->getType() == mode->getType())
        {
            mode->turnOn(newDevices[i]);
        }
    }

#ifdef DEBUG

    fprintf(stderr, "================\n");
    for (DeviceMap::const_iterator it = devices.begin(); it != devices.end(); ++it)
    {
        fprintf(stderr, "Device: %s:\n", UserMode::enumToString(it->first).c_str());
        for (std::multimap<int, UserMode*>::const_iterator innerIt = it->second.begin(); innerIt != it->second.end(); ++innerIt)
        {
            fprintf(stderr, "%d: %s\n", innerIt->first, UserMode::enumToString(innerIt->second->getType()).c_str());
        }
    }

#endif
}

void ModeManager::removeMode(UserMode::Mode toRemove)
{
    fprintf(stderr, "Called remove mode (for %s)\n", enumToString(toRemove).c_str());
    if (!isActive(toRemove))
    {
        fprintf(stderr, "Mode is not active. (%s)\n", enumToString(toRemove).c_str());
        return;
    }

    UserMode* mode = getUserMode(toRemove);

    std::vector<UserMode::Device> oldDevices = mode->getDevices();

    for (size_t i = 0; i < oldDevices.size(); ++i)
    {
        fprintf(stderr, "Device %s\n", enumToString(oldDevices[i]).c_str());

        std::multimap<int, UserMode*>::iterator last = devices[oldDevices[i]].end();
        --last;

        bool wasFirst = false;

        fprintf(stderr, "First mode for this device was %s\n", enumToString(last->second->getType()).c_str());

        if (last->second->getType() == mode->getType())
        {
            fprintf(stderr, "Calling turn off\n");
            last->second->turnOff(oldDevices[i]);
            wasFirst = true;
        }

        for (std::multimap<int, UserMode*>::const_iterator it = devices[oldDevices[i]].begin(); it != devices[oldDevices[i]].end(); ++it)
        {
            if (it->second->getType() == mode->getType())
            {
                devices[oldDevices[i]].erase(it);
                break;
            }
        }

        if (wasFirst)
        {
            std::multimap<int, UserMode*>::iterator last = devices[oldDevices[i]].end();
            --last;
            fprintf(stderr, "Turning on next mode after remove (Turning on %s)\n", enumToString(last->second->getType()).c_str());
            last->second->turnOn(oldDevices[i]);
        }
    }

#ifdef DEBUG

    fprintf(stderr, "================\n");
    for (DeviceMap::const_iterator it = devices.begin(); it != devices.end(); ++it)
    {
        fprintf(stderr, "Device: %s:\n", UserMode::enumToString(it->first).c_str());
        for (std::multimap<int, UserMode*>::const_iterator innerIt = it->second.begin(); innerIt != it->second.end(); ++innerIt)
        {
            fprintf(stderr, "%d: %s\n", innerIt->first, UserMode::enumToString(innerIt->second->getType()).c_str());
        }
    }

#endif

}

bool ModeManager::isActive(UserMode::Mode type) const
{
    for (DeviceMap::const_iterator it = devices.begin(); it != devices.end(); ++it)
    {
        for (std::multimap<int, UserMode*>::const_iterator innerIterator = it->second.begin(); innerIterator != it->second.end(); ++innerIterator)
        {
            if (innerIterator->second->getType() == type)
            {
                return true;
            }
        }
    }
    return false;
}

UserMode* ModeManager::getUserMode(UserMode::Mode type)
{
    switch (type)
    {
    case UserMode::eOffMode:
        return new OffMode();
        break;
    case UserMode::ePCMode:
        return new PCMode();
        break;
    }
    return 0;
}

std::string ModeManager::enumToString(const UserMode::Device &device)
{
    switch (device)
    {
    case UserMode::eBeamer:
        return "Beamer";
        break;
    case UserMode::eHMI:
        return "HMI";
        break;
    case UserMode::eStereo:
        return "Stereo";
        break;
    default:
        return "Unidentified device";
        break;
    }
}

std::string ModeManager::enumToString(const UserMode::Mode &type)
{
    switch (type)
    {
    case UserMode::eOffMode:
        return "Off Mode";
        break;
    case UserMode::ePCMode:
        return "PC Mode";
        break;
    case UserMode::eDVDMode:
        return "DVD Mode";
        break;
    default:
        return "Unidentified Mode";
        break;
    }
}
