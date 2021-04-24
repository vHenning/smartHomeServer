#include "ModeManager.h"
#include "UserMode.h"
#include "modes/DVDMode.h"
#include "modes/OffMode.h"
#include "modes/PCMode.h"
#include "modes/IlluminateMode.h"
#include "../LED/LEDManager.h"

ModeManager* ModeManager::getInstance()
{
    static ModeManager instance;
    return &instance;
}

ModeManager::ModeManager()
{
}

void ModeManager::addMode(UserMode::Mode toAdd)
{
    UserMode* mode = getUserMode(toAdd);

    // TODO use mode->getDevices only once

    if (isActive(mode->getType()))
    {
        // If the mode is already active, push it to the front of all modes with the same dominance
        std::vector<UserMode::Device> modeDevices = mode->getDevices();
        for (size_t i = 0; i < modeDevices.size(); ++i)
        {
            // If mode is not already first (active)
            ModeMap::iterator first = devices[modeDevices[i]].end();
            --first;
            if (first->second->getType() != mode->getType())
            {
                // Iterate through every mode for this device until this mode is found
                for (ModeMap::iterator it = devices[modeDevices[i]].begin(); it != devices[modeDevices[i]].end(); ++it)
                {
                    if (it->second->getType() == mode->getType())
                    {
                        // If this mode is found, remove it and add it again. std::multimap automatically adds it at the desired position
                        UserMode* currentMode = it->second;
                        devices[modeDevices[i]].erase(it);
                        devices[modeDevices[i]].insert(std::make_pair(mode->getDominance(modeDevices[i]), currentMode));

                        // If this mode is now the first mode, turn it on
                        first = devices[modeDevices[i]].end();
                        --first;
                        if (first->second->getType() == mode->getType())
                        {
                            first->second->turnOn(modeDevices[i]);
                        }
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // If the mode is not active already, add it for every device

        std::vector<UserMode::Device> newDevices = mode->getDevices();

        for (size_t i = 0; i < newDevices.size(); ++i)
        {
            // Insert new mode
            devices[newDevices[i]].insert(std::pair<int, UserMode*>(mode->getDominance(newDevices[i]), mode));

            // If the first mode is the added mode, turn it on
            std::multimap<int, UserMode*>::const_iterator last = devices[newDevices[i]].end();
            --last;
            if (last->second->getType() == mode->getType())
            {
                mode->turnOn(newDevices[i]);
            }
        }
    }

    printDeviceMap();
}

void ModeManager::removeMode(UserMode::Mode toRemove)
{
    if (!isActive(toRemove))
    {
#ifdef DEBUG
        fprintf(stderr, "Mode is not active. (%s)\n", UserMode::enumToString(toRemove).c_str());
#endif
        return;
    }

    UserMode* mode = getUserMode(toRemove);

    std::vector<UserMode::Device> oldDevices = mode->getDevices();

    for (size_t i = 0; i < oldDevices.size(); ++i)
    {
        // Find the last mode in the multimap (which is the most dominant mode)
        std::multimap<int, UserMode*>::iterator last = devices[oldDevices[i]].end();
        --last;

        // True if the removed mode was the most dominant (--> activated) mode for this device
        bool wasFirst = last->second->getType() == mode->getType();

        // Actually remove the mode from this device
        for (std::multimap<int, UserMode*>::const_iterator it = devices[oldDevices[i]].begin(); it != devices[oldDevices[i]].end(); ++it)
        {
            if (it->second->getType() == mode->getType())
            {
                devices[oldDevices[i]].erase(it);
                break;
            }
        }

        // If the activated mode was removed, the next most dominant mode (if there is one left) is turned on
        if (wasFirst && devices[oldDevices[i]].size())
        {
            std::multimap<int, UserMode*>::iterator newLast = devices[oldDevices[i]].end();
            --newLast;
            newLast->second->turnOn(oldDevices[i]);
        }
    }

    printDeviceMap();
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
    case UserMode::eIlluminateBedroomMode:
        return new IlluminateMode(LEDManager::getInstance()->getUnit(LEDManager::eBedroom), UserMode::eIlluminateBedroomMode);
        break;
    case UserMode::eReadingMode:
        return new IlluminateMode(LEDManager::getInstance()->getUnit(LEDManager::eBedroomReading), UserMode::eReadingMode);
        break;
    case UserMode::eDVDMode:
        return 0;
        break;
    }
    return 0;
}

void ModeManager::printDeviceMap()
{
#ifdef DEBUG

    for (DeviceMap::const_iterator it = devices.begin(); it != devices.end(); ++it)
    {
        fprintf(stderr, "Device: %s:\n", UserMode::enumToString(it->first).c_str());
        for (std::multimap<int, UserMode*>::const_iterator innerIt = it->second.begin(); innerIt != it->second.end(); ++innerIt)
        {
            fprintf(stderr, "%+011d: %s\n", innerIt->first, UserMode::enumToString(innerIt->second->getType()).c_str());
        }
    }
    fprintf(stderr, "================\n");

#endif
}
