#include "LEDManager.h"

LEDManager* LEDManager::getInstance()
{
    static LEDManager manager;
    return &manager;
}

LEDManager::LEDManager() : socket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 8002))
{
    units[eBedroom] = new LEDController("192.168.0.81", 0, "LED/bedroom", std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eBedroomReading] = new LEDController("192.168.0.82", 0, "LED/bedroomReading", std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eTV] = new LEDController("192.168.0.81", 1, "LED/tv", std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eDesk] = new LEDController("192.168.0.83", 0, "LED/desk", std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eKitchenSink] = new LEDController("192.168.0.84", 0, "LED/kitchenSink", std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    units[eDiningRoom] = new LEDController("192.168.0.81", 0, "LED/diningRoom", std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eLivingRoomTV] = new LEDController("192.168.0.81", 1, "LED/livingRoomTV", std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    units[eLivingRoom] = new LEDController("192.168.0.82", 0, "LED/livingRoom", std::bind(&LEDManager::sendBuffer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

LEDController* LEDManager::getUnit(const Unit &unit)
{
    return units[unit];
}

void LEDManager::sendBuffer(const boost::asio::ip::udp::endpoint &destination, const unsigned char *buffer, const size_t &size)
{
    // TODO separate send thread#
    for (int i = 0; i < 2; ++i)
    {
        std::lock_guard<std::mutex> m(socketMutex);
        socket.wait(boost::asio::ip::udp::socket::wait_write);
        socket.send_to(boost::asio::buffer(buffer, size), destination);
        usleep(10000);
    }
}
