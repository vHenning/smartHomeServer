#ifndef SWITCH_H
#define SWITCH_H

#include <string>
#include <functional>
#include <vector>

class Switch
{
public:
    Switch(std::string name);

    void addHandler(std::function<void(bool)> handler);

private:

    void mqttHandler(std::string receivedTopic, std::string message);

private:
    std::string topic;

    bool on;

    std::vector<std::function<void(bool)> > handlers;
};

#endif // SWITCH_H
