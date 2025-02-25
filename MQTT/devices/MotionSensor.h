#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

#include <string>
#include <vector>
#include <functional>

namespace mqtt
{

class MotionSensor
{
public:
    MotionSensor(std::string address);

    /**
     * @brief Add Motion started/stopped handler. Reset time (Time between no more motion detected and calling handler(false)) is set by configuring the device.
     * 
     * @param handler Function that takes the state of motion (true = motion detected, false = no motion detected)
     */
    void addMotionHandler(std::function<void(bool)> handler);

    /**
     * @brief Add handler for changing illuminance
     * 
     * @param threshold Call handler each time illuminance crosses this threshold value
     * @param hysteresis The amount that the value must exceed the threshold (in both directions) in order to trigger calling the handler
     */
    void addIlluminanceHandler(std::function<void(double)>, double threshold, double hysteresis);

    bool getMotion() const;

    double getIlluminance() const;

private:
    void mqttHandler(std::string topic, std::string message);

private:
    std::string address;

    bool motion;

    /// periodically updated illuminance [lux]
    double illuminance;

    std::vector<std::function<void(bool)> > motionHandlers;
    
    struct IlluminanceHandler
    {
        IlluminanceHandler(std::function<void(double)> handler, double threshold, double hysteresis) : handler(handler), threshold(threshold), hysteresis(hysteresis), aboveThreshold(false) {}

        std::function<void(double)> handler;
        double threshold;
        double hysteresis;

        // Is our state currently above the threshold?
        bool aboveThreshold;
        
        void illuminanceChanged(double illuminance);
    };

    /// bool: true if the current state is above the threshold
    std::vector<IlluminanceHandler> illuminanceHandlers;
};

} // namespace mqtt

#endif // MOTION_SENSOR_H
