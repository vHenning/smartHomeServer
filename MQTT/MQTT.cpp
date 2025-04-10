#include "MQTT.h"

#include <mqtt/client.h>

MQTT* MQTT::getInstance()
{
    static MQTT instance;
    return &instance;
}

MQTT::MQTT()
    : client("mqtt://192.168.0.80", "smarthomeserver")
{
    client.connect();

    runnerThread = std::thread(&MQTT::runner, this);
}

void MQTT::addHandler(std::string topic, std::function<void(std::string, std::string)> function)
{
    handlers.push_back(function);

    client.subscribe(topic, 1);
}

void MQTT::publish(std::string topic, std::string message)
{
    mqtt::message msg(mqtt::string_ref(topic), static_cast<void*>(message.data()), message.length());
    client.publish(msg);
}

void MQTT::runner()
{
    while (true)
    {
        mqtt::const_message_ptr msg = client.consume_message();

        for (size_t i = 0; i < handlers.size(); ++i)
        {
            handlers[i](msg->get_topic(), msg->get_payload_str());
        }
    }
}