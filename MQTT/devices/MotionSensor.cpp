#include "MotionSensor.h"

#include <nlohmann/json.hpp>

#include "../MQTT.h"

namespace mqtt
{

MotionSensor::MotionSensor(std::string address)
    : address(address)
    , motion(false)
    , illuminance(0)
{
    MQTT::getInstance()->addHandler(address, std::bind(&MotionSensor::mqttHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void MotionSensor::addMotionHandler(std::function<void(bool)> handler)
{
    motionHandlers.push_back(handler);
}

void MotionSensor::addIlluminanceHandler(std::function<void(double)> handler, double threshold, double hysteresis)
{
    illuminanceHandlers.push_back(IlluminanceHandler(handler, threshold, hysteresis));
}

void MotionSensor::mqttHandler(std::string topic, std::string message)
{
    using json = nlohmann::json;

    if (topic != address) return;

    json data = json::parse(message);

    json motionJson = data["service_data"]["motion"];
    json illuminanceJson = data["service_data"]["illuminance"];

    if (illuminanceJson.is_number())
    {
        // The shelly motion sensor illuminance has a scale factor of 0.01
        double localIlluminance = illuminanceJson;
        illuminance = localIlluminance * 100;

        for (size_t i = 0; i < illuminanceHandlers.size(); ++i)
        {
            illuminanceHandlers[i].illuminanceChanged(illuminance);
        }
    }
    
    if (!motionJson.is_number()) return;
    bool localMotion = motionJson == 1 ? true : false;

    if (localMotion != motion)
    {
        motion = localMotion;
        for (size_t i = 0; i < motionHandlers.size(); ++i)
        {
            motionHandlers[i](motion);
        }
    }
}

void MotionSensor::IlluminanceHandler::illuminanceChanged(double illuminance)
{
    bool notify =
    (aboveThreshold && illuminance < threshold - hysteresis) ||
    (!aboveThreshold && illuminance > threshold + hysteresis);

    if (notify)
    {
        aboveThreshold = !aboveThreshold;

        handler(illuminance);
    }
}

bool MotionSensor::getMotion() const { return motion; }

double MotionSensor::getIlluminance() const { return illuminance; }

} // namespace mqtt
