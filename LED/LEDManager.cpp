#include "LEDManager.h"

LEDManager* LEDManager::getInstance()
{
    static LEDManager manager;
    return &manager;
}

LEDManager::LEDManager() : socket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 8002))
{
    units[eBedroom] = new LEDController("192.168.0.117", 0, std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eBedroomReading] = new LEDController("192.168.0.117", 1, std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eKitchenRGB] = new LEDController("192.168.0.196", 0, std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eKitchenW] = new LEDController("192.168.0.196", 1, std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

LEDController* LEDManager::getUnit(const Unit &unit)
{
    return units[unit];
}

void LEDManager::sendBuffer(const boost::asio::ip::udp::endpoint &destination, const unsigned char *buffer, const size_t &size)
{
    // TODO separate send thread
    std::lock_guard<std::mutex> m(socketMutex);
    socket.send_to(boost::asio::buffer(buffer, size), destination);
    usleep(10000);
}
