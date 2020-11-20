#ifndef BENQ_SERVER_H
#define BENQ_SERVER_H

#include <boost/asio/serial_port.hpp>
#include <condition_variable>
#include <thread>

class BenQServer
{
public:

    enum PowerState
    {
        ePowerInvalid
        , ePowerOn
        , ePowerStandby
    };

    enum Source
    {
        eSourceInvalid
        , eVGA
        , eHDMI1
        , eHDMI2
    };

    static BenQServer* getInstance();

    virtual ~BenQServer();

    virtual PowerState getPowerState();
    virtual PowerState setPowerState(PowerState value);

    virtual Source getSource();
    virtual Source setSource(Source value);

protected:
    char getChar();

    std::string sendCommand(const std::string& command);

    std::string sendActualCommand(const std::string& command);

    void waitForReceive(const int& millis);

    std::string receivePacket();

private:
    BenQServer();
    std::thread receiveThread;
    void runReceive();

    std::condition_variable waitReceive;
    std::mutex conditionalMutex;
    std::mutex receivedMutex;
    char received;
    bool newReceived;
    bool waiting;

    boost::asio::serial_port* port;
    boost::asio::io_context context;

    bool running;
};

#endif
