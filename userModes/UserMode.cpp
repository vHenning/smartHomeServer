#include "UserMode.h"

std::string UserMode::enumToString(const Device &device)
{
    switch (device)
    {
    case eStereo:
        return "Stereo";
    case eBeamer:
        return "Beamer";
    case eHMI:
        return "HMI";
    case eLEDBedroom:
        return "Background LED Bedroom";
    case eLEDBedroomReading:
        return "Reading LED Bedroom";
    }
    return "Unknown";
}

std::string UserMode::enumToString(const Mode &mode)
{
    switch (mode)
    {
    case eOffMode:
        return "Off";
    case ePCMode:
        return "PC";
    case eDVDMode:
        return "DVD";
    case eIlluminateBedroomMode:
        return "Background Illumination Bedroom";
    case eReadingMode:
        return "Reading Bedroom";
    }
    return "Unknown";
}

void UserMode::turnOn(const Device &device)
{
#ifdef DEBUG
    fprintf(stderr, "Turn on %s mode on device %s\n", enumToString(getType()).c_str(), enumToString(device).c_str());
#endif

#ifndef DRYRUN
    DeviceMap::iterator it = devices.find(device);
    if (it != devices.end())
    {
        it->second.second();
    }
#endif
}

UserMode::UserMode(Mode type) : type(type)
{
}

std::vector<UserMode::Device> UserMode::getDevices() const
{
    std::vector<Device> sendback;

    for (DeviceMap::const_iterator it = devices.begin(); it != devices.end(); it++)
    {
        sendback.push_back(it->first);
    }

    return sendback;
}

int UserMode::getDominance(Device device) const
{
    DeviceMap::const_iterator it = devices.find(device);
    if (it != devices.end())
    {
        return it->second.first;
    }
    fprintf(stderr, "Device not found.\n");
    return 0;
}

UserMode::Mode UserMode::getType() const
{
    return type;
}
