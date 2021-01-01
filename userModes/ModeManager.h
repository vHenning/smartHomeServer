#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <map>
#include "UserMode.h"

class ModeManager
{
public:
    ModeManager();

    void addMode(UserMode::Mode toAdd);
    void removeMode(UserMode::Mode toRemove);

    bool isActive(UserMode::Mode type) const;

    static std::string enumToString(const UserMode::Mode &type);
    static std::string enumToString(const UserMode::Device &device);

private:

    UserMode* getUserMode(UserMode::Mode type);

    typedef std::map<UserMode::Device, std::multimap<int, UserMode*> > DeviceMap;
    DeviceMap devices;


};

#endif // MODEMANAGER_H
