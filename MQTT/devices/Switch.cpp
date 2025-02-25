#include "Switch.h"

#include <nlohmann/json.hpp>

#include "../MQTT.h"

Switch::Switch(std::string name)
    : topic(name + "/status/input:0")
    , on(false)
{
    MQTT::getInstance()->addHandler(topic, std::bind(&Switch::mqttHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void Switch::addHandler(std::function<void(bool)> handler)
{
    handlers.push_back(handler);
}

void Switch::mqttHandler(std::string receivedTopic, std::string message)
{
    using json = nlohmann::json;

    if (receivedTopic != topic) return;

    json parsedMessage = json::parse(message);
    json onJson = parsedMessage["state"];

    if (onJson.is_boolean())
    {
        bool tempOn = onJson;
        if (on != tempOn)
        {
            on = tempOn;

            for (size_t i = 0; i < handlers.size(); ++i)
            {
                handlers[i](on);
            }
        }
    }
}
