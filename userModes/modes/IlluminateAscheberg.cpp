#include "IlluminateAscheberg.h"
#include "../../LED/LEDManager.h"
#include "../../LED/Rayleigh.h"
#include <math.h>

IlluminateAscheberg::IlluminateAscheberg(Mode mode) : UserMode(mode)
{
    switch (mode)
    {
        case UserMode::eIlluminateDiningMode:
            devices[eLEDDining] = std::make_pair(10, std::bind(&IlluminateAscheberg::turnOn, this, LEDManager::getInstance()->getUnit(LEDManager::eDiningRoom)));
            break;
        case UserMode::eIlluminateLivingMode:
            devices[eLEDLiving] = std::make_pair(10, std::bind(&IlluminateAscheberg::turnOn, this, LEDManager::getInstance()->getUnit(LEDManager::eLivingRoom)));
            devices[eLEDLivingTV] = std::make_pair(10, std::bind(&IlluminateAscheberg::turnOn, this, LEDManager::getInstance()->getUnit(LEDManager::eLivingRoomTV)));
            break;
        default:
            break;
    }

}

void IlluminateAscheberg::turnOn(LEDController* led)
{
    double zenith = 85;
    double brightness = 0.45;
    std::vector<double> rgb = Rayleigh::getRGB((zenith / 180.0) * M_PI);
    uint8_t red = rgb[0] * 0xFF;
    uint8_t green = rgb[1] * 0xFF;
    uint8_t blue = rgb[2] * 0xFF;
    double temp = Rayleigh::getColorTemperature((zenith / 180.0) * M_PI);

    if (red > 0 || green > 0 || blue > 0)
    {
        led->setColor(red, green, blue);
    }
    led->setWhiteTemp(std::max(std::min(temp, 6000.0), 3000.0));

    led->setDim(brightness);
    led->setWhiteDim(brightness);

    led->turnOn(true);
}