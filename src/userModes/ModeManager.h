#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <map>
#include "UserMode.h"

class ModeManager
{
public:

    static ModeManager* getInstance();

    void addMode(UserMode::Mode toAdd);
    void removeMode(UserMode::Mode toRemove);

    bool isActive(UserMode::Mode type) const;
    UserMode::Mode getActiveMode(const UserMode::Device &device);

private:
    ModeManager();

    UserMode* getUserMode(UserMode::Mode type);

    /**
     * Prints the UserMode queue for every device
     */
    void printDeviceMap();

    // The most dominant mode is situated at the end of each ModeMap.
    typedef std::multimap<int, UserMode*> ModeMap;
    typedef std::map<UserMode::Device, ModeMap> DeviceMap;
    DeviceMap devices;
};

#endif // MODEMANAGER_H
