#ifndef ONKYOSERVER_H
#define ONKYOSERVER_H

//#define DEBUG

#include <boost/asio.hpp>

class OnkyoServer
{
public:

    static OnkyoServer* getInstance();

    enum PowerState
    {
        ePowerInvalid,
        eOn,
        eStandby
    };

    enum Source
    {
        eSourceInvalid,
        eNet,
        ePC,
        eBDDVD
    };


    ~OnkyoServer();

    PowerState getPower();
    PowerState setPower(PowerState value);

    Source getSource();
    Source setSource(Source value);

    int getVolume();
    int setVolume(int value);

#ifdef DEBUG
    void sendManual(std::string command);
#endif

private:
    OnkyoServer();
    enum MessageType
    {
        eOther,
        ePower,
        eVolume,
        eSource
    };

    void send(const char* data, size_t size);

    void readMessage(const std::string& message);

    void runReceive();

    void powerMessage(const std::string& data);
    PowerState powerState;

    void sourceMessage(const std::string& data);
    Source source;

    void volumeMessage(const std::string& data);
    int volume;

    bool running;

    const boost::asio::ip::udp::endpoint partner;

    std::thread receiveThread;

    boost::asio::ip::udp::socket* socket;
    boost::asio::io_service service;
};

#endif // ONKYOSERVER_H
