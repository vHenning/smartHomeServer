#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include "LEDController.h"

class LEDManager
{
public:
    enum Unit
    {
        eBedroom = 0
        , eBedroomReading = 1
        , eTV = 2
        , eUnitCount
    };

    static LEDManager* getInstance();

    LEDController* getUnit(const Unit &unit);

private:
    LEDManager();

    LEDController* units[eUnitCount];

    void sendBuffer(const boost::asio::ip::udp::endpoint &destination, const unsigned char *buffer, const size_t &size);

    std::mutex socketMutex;
    boost::asio::io_service service;
    boost::asio::ip::udp::socket socket;
};

#endif // LEDMANAGER_H
