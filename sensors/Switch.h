#ifndef SWITCH_H
#define SWITCH_H

#include <boost/asio/ip/udp.hpp>
#include <map>
#include <mutex>
#include <thread>
#include <functional>

/**
 * This is a device that sends a 64 bit integer representing its state.
 * Current implementation only calls listeners if the value goes below the maximum.
 * This will need some refactoring to make more use of this interface.
 */
class Switch
{
public:
    Switch();

    ~Switch();

    std::map<uint32_t, std::vector<std::function<void()> > > listeners;

private:

    struct __attribute__((__packed__)) ValueMessage
    {
        uint32_t id;
        uint32_t unitID;
        uint8_t type;
        uint64_t value;
    };

    void run();

    void runUnitThread(const uint32_t unit);

    struct Unit
    {
        Unit() : value(0), previousValue(0) {}
        std::thread thread;
        uint64_t value;
        uint64_t previousValue;
        std::mutex valueMutex;
    };

    std::thread receiveThread;
    std::map<uint32_t, Unit> units;


    boost::asio::io_context context;
    boost::asio::ip::udp::socket* socket;
};

#endif // SWITCH_H
