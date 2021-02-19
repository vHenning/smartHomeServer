#include "ModeManager.h"
#include "UserMode.h"
#include "modes/DVDMode.h"
#include "modes/OffMode.h"
#include "modes/PCMode.h"
#include "modes/IlluminateMode.h"
#include "../LED/LEDManager.h"

ModeManager::ModeManager()
{
}

void ModeManager::addMode(UserMode::Mode toAdd)
{
    UserMode* mode = getUserMode(toAdd);

    // TODO use mode->getDevices only once

    if (isActive(mode->getType()))
    {
        std::vector<UserMode::Device> modeDevices = mode->getDevices();
        for (size_t i = 0; i < modeDevices.size(); ++i)
        {
            // if first mode and added mode have same dominance
            ModeMap::iterator first = devices[modeDevices[i]].end();
            --first;
            // If mode is not already first (active)
            if (first->second->getType() != mode->getType())
            {
                if (first->first == mode->getDominance(modeDevices[i]))
                {
                    first->second->turnOff(modeDevices[i]);
                }

                for (ModeMap::iterator it = devices[modeDevices[i]].begin(); it != devices[modeDevices[i]].end(); ++it)
                {
                    if (it->second->getType() == mode->getType())
                    {
                        UserMode* currentMode = it->second;
                        devices[modeDevices[i]].erase(it);
                        devices[modeDevices[i]].insert(std::make_pair(mode->getDominance(modeDevices[i]), currentMode));

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
        std::vector<UserMode::Device> newDevices = mode->getDevices();

        for (size_t i = 0; i < newDevices.size(); ++i)
        {
            UserMode* previousFirstMode = 0;
            if (devices[newDevices[i]].size())
            {
                ModeMap::iterator it = devices[newDevices[i]].end();
                --it;
                previousFirstMode = it->second;
            }

            devices[newDevices[i]].insert(std::pair<int, UserMode*>(mode->getDominance(newDevices[i]), mode));
            // "last" is really the first mode (with highest dominance)
            std::multimap<int, UserMode*>::const_iterator last = devices[newDevices[i]].end();
            --last;

            // If the initially first mode is not first anymore, turn it off
            if (previousFirstMode && previousFirstMode->getType() != last->second->getType())
            {
                previousFirstMode->turnOff(newDevices[i]);
            }
            // If the first mode is the added mode, turn it on
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
            if (last->second == 0)
            {
                fprintf(stderr, "Last was 0\n");
            }
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

        if (wasFirst && devices[oldDevices[i]].size())
        {
            std::multimap<int, UserMode*>::iterator last = devices[oldDevices[i]].end();
            --last;
            fprintf(stderr, "Turning on next mode after remove (Turning on %s)\n", enumToString(last->second->getType()).c_str());
            last->second->turnOn(oldDevices[i]);
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
            fprintf(stderr, "%d: %s\n", innerIt->first, UserMode::enumToString(innerIt->second->getType()).c_str());
        }
    }
    fprintf(stderr, "================\n");

#endif
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
