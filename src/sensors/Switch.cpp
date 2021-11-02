#include "Switch.h"
#include <math.h>

Switch::Switch()
    : socket(new boost::asio::ip::udp::socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 8007)))
{
    receiveThread = std::thread(&Switch::run, this);
}

Switch::~Switch()
{
    receiveThread.join();
    for (size_t i = 0; i < units.size(); ++i)
    {
        units[i].thread.join();
    }
    delete socket;
}

void Switch::run()
{
    while(true)
    {
        socket->wait(boost::asio::ip::udp::socket::wait_read);
        std::vector<char> bufferVector;
        bufferVector.resize(socket->available());
        boost::asio::ip::udp::endpoint from;
        size_t size = socket->receive_from(boost::asio::buffer(bufferVector), from);

        if (size == sizeof(ValueMessage))
        {
            ValueMessage message;
            memcpy(&message, bufferVector.data(), sizeof(ValueMessage));

            if (units.find(message.unitID) == units.end())
            {
                uint32_t id = message.unitID;
                units[id].thread = std::thread(&Switch::runUnitThread, this, id);
            }

            std::lock_guard<std::mutex> m(units[message.unitID].valueMutex);
            units[message.unitID].value = message.value;
        }
    }
}

void Switch::runUnitThread(const uint32_t unit)
{
    while (true)
    {
        uint64_t messageValue = 0;
        {
            std::lock_guard<std::mutex> m(units[unit].valueMutex);
            messageValue = units[unit].value;
        }

        double value = static_cast<double>(messageValue) / std::numeric_limits<uint64_t>::max();
        double previousValue = static_cast<double>(units[unit].previousValue) / std::numeric_limits<uint64_t>::max();

        if (value < 1 && fabs(static_cast<double>(previousValue - 1)) < std::numeric_limits<double>::epsilon())
        {
            for (size_t i = 0; i < listeners[unit].size(); ++i)
            {
                listeners[unit][i]();
            }
        }

        units[unit].previousValue = messageValue;

        usleep(10000);
    }
}
