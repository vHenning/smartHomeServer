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
    default:
        return "Unknown";
    }
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
    default:
        return "Unknown";
    }
}

UserMode::UserMode(Mode type) : type(type)
{
}

//void UserMode::turnOn(const Device &device)
//{
//#ifdef DEBUG
//    fprintf(stderr, "Turn on generic UserMode\n");
//#endif
//}

//void UserMode::turnOff(const Device &device)
//{
//#ifdef DEBUG
//    fprintf(stderr, "Turn off generic UserMode\n");
//#endif
//}

std::vector<UserMode::Device> UserMode::getDevices() const
{
    std::vector<Device> sendback;

    for (std::map<Device, int>::const_iterator it = devices.begin(); it != devices.end(); it++)
    {
        sendback.push_back(it->first);
    }

    return sendback;
}

int UserMode::getDominance(Device device) const
{
    std::map<UserMode::Device, int>::const_iterator it = devices.find(device);
    if (it != devices.end())
    {
        return it->second;
    }
    fprintf(stderr, "Device not found.\n");
    return 0;
}

UserMode::Mode UserMode::getType() const
{
    return type;
}
