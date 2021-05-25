#ifndef USERMODE_H
#define USERMODE_H

#include <map>
#include <vector>
#include <functional>

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
        , eKodi
        , eLEDKitchenRGB
        , eLEDKitchenWhite
    };

    enum Mode
    {
        eOffMode
        , ePCMode
        , eDVDMode
        , eIlluminateBedroomMode
        , eReadingMode
        , eKodiMode
        , eIlluminateKitchenMode
    };

    static std::string enumToString(const Device &device);
    static std::string enumToString(const Mode &mode);

    void turnOn(const Device &device);
    std::vector<Device> getDevices() const;
    int getDominance(Device device) const;
    Mode getType() const;

protected:

    UserMode(Mode type);

    typedef std::map<Device, std::pair<int, std::function<void (void)> > > DeviceMap;
    DeviceMap devices;

private:
    UserMode() {}
    Mode type;

};

#endif // USERMODE_H
