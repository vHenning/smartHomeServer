#ifndef USERMODE_H
#define USERMODE_H

#include <map>
#include <vector>

class UserMode
{
public:
    enum Device
    {
        eHMI
        , eStereo
        , eBeamer
        , eLEDBedroom
        , eLEDBedroomReading
    };

    enum Mode
    {
        eOffMode
        , ePCMode
        , eDVDMode
        , eIlluminateBedroomMode
        , eReadingMode
    };

    static std::string enumToString(const Device &device);
    static std::string enumToString(const Mode &mode);

    virtual void turnOn(const Device &device) = 0;
    virtual void turnOff(const Device &device) = 0;
    std::vector<Device> getDevices() const;
    int getDominance(Device device) const;
    Mode getType() const;

protected:
    UserMode(Mode type);
    // TODO: Add functions for turning on and off each device (instead of turnOn() turnOff())
    std::map<Device, int> devices;

private:
    UserMode() {}
    Mode type;

};

#endif // USERMODE_H
