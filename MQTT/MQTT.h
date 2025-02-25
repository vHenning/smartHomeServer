#ifndef MQTT_H
#define MQTT_H

#include <string>
#include <functional>
#include <thread>
#include <mqtt/client.h>

class MQTT
{
public:
    static MQTT* getInstance();
    
    /**
     * @brief Add a message handler and listen to given topic (with wildcards)
     * The handler is always called when a message is received regardless of the topic because I did not want to implement wildcard matching.
     * 
     * @param topic 
     * @param function 
     */
    void addHandler(std::string topic, std::function<void(std::string, std::string)> function);
    
private:
    MQTT();

    mqtt::client client;

    std::vector<std::function<void(std::string, std::string)> > handlers;
    
    void runner(void);
    std::thread runnerThread;
};

#endif // MQTT_H
