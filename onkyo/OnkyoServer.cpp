#include "OnkyoServer.h"

#include <boost/asio/ip/udp.hpp>

OnkyoServer::OnkyoServer() :
    powerState(ePowerInvalid),
    source(eSourceInvalid),
    volume(-1),
    running(true),
    partner(boost::asio::ip::make_address_v4("127.0.0.1"), 5500)
{
    const int PORT = 5501;

    socket = new boost::asio::ip::udp::socket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), PORT));

    receiveThread = std::thread(&OnkyoServer::runReceive, this);
}

OnkyoServer* OnkyoServer::getInstance()
{
    static OnkyoServer instance;
    return &instance;
}

OnkyoServer::~OnkyoServer()
{
    running = false;
    socket->cancel();
    receiveThread.join();
}

void OnkyoServer::send(const char *data, size_t size)
{
    std::vector<char> buffer;
    buffer.resize(size);
    memcpy(buffer.data(), data, size);
    socket->send_to(boost::asio::buffer(buffer), partner);
}

void OnkyoServer::runReceive()
{
    while (socket == 0 || !socket->is_open())
    {
        usleep(1000);
    }
    fprintf(stderr, "Open\n");
    while(running)
    {
        socket->wait(boost::asio::ip::udp::socket::wait_read);
        size_t avail = socket->available();
        std::vector<char> bufferVector;
        bufferVector.resize(avail);
        size_t received = socket->receive(boost::asio::buffer(bufferVector));

        std::string message(bufferVector.data(), std::min(received, bufferVector.size()));
//        fprintf(stderr, "Received: %s\n", message.c_str());
        if (received == avail)
        {
            readMessage(message);
        }
    }
}

void OnkyoServer::readMessage(const std::string& message)
{
    std::string substring = message.substr(0, 3);

    if (substring == "PWR")
    {
        powerMessage(message.substr(3, 2));
    }
    else if (substring == "SLI")
    {
        sourceMessage(message.substr(3, 2));
    }
    else if (substring == "MVL")
    {
        volumeMessage(message.substr(3, 2));
    }
}

void OnkyoServer::volumeMessage(const std::string &data)
{
    int scannedVolume = 0;
    sscanf(data.c_str(), "%X", &scannedVolume);

    if (scannedVolume <= 100 && scannedVolume >= 0)
    {
        volume = scannedVolume;
    }
}

int OnkyoServer::getVolume()
{
    if (volume == -1)
    {
        send("MVLQSTN\n", 8);

        for (int i = 0; i < 10000; ++i)
        {
            if (volume != -1)
            {
                break;
            }
            usleep(100);
        }
    }

    return volume;
}

int OnkyoServer::setVolume(int value)
{
    if (value < 0 || value > 100)
    {
        fprintf(stderr, "Warning: OnkyoServer: Wrong volume set. Volume must be between 0 and 100.\n");
    }
    else
    {
        const size_t MESSAGE_SIZE = 6;
        char* message = new char[MESSAGE_SIZE];

        sprintf(message, "MVL%.2X\n", value);

        send(message, MESSAGE_SIZE);
    }

    for (int i = 0; i < 10000; ++i)
    {
        if (volume == value)
        {
            break;
        }
        usleep(100);
    }

    return volume;
}

OnkyoServer::PowerState OnkyoServer::getPower()
{
    if (powerState == ePowerInvalid)
    {
        send("PWRQSTN\n", 8);

        for (int i = 0; i < 10000; ++i)
        {
            if (powerState != ePowerInvalid)
            {
                break;
            }
            usleep(100);
        }
    }

    return powerState;
}

OnkyoServer::PowerState OnkyoServer::setPower(PowerState value)
{
    switch (value)
    {
    case eOn:
        send("PWR01\n", 6);
        break;
    case eStandby:
        send("PWR00\n", 6);
        break;
    case ePowerInvalid:
        fprintf(stderr, "Warning: Power cannot be set invalid.\n");
        break;
    }

    for (int i = 0; i < 10000; ++i)
    {
        if (powerState == value)
        {
            break;
        }
        usleep(100);
    }

    return powerState;
}

void OnkyoServer::powerMessage(const std::string& data)
{
    if (data.compare("01") == 0)
    {
        powerState = eOn;
    }
    else if (data.compare("00") == 0)
    {
        powerState = eStandby;
    }
    else
    {
        powerState = ePowerInvalid;
    }
}

OnkyoServer::Source OnkyoServer::setSource(Source value)
{
    switch (value)
    {
    case ePC:
        send("SLI05\n", 6);
        break;
    case eNet:
        send("SLI2B\n", 6);
        break;
    case eBDDVD:
    case eSourceInvalid:
        fprintf(stderr, "Warning: Source cannot be set invalid.\n");
        break;
    }

    for (int i = 0; i < 10000; ++i)
    {
        if (source == value)
        {
            break;
        }
        usleep(100);
    }

    return source;

}

OnkyoServer::Source OnkyoServer::getSource()
{
    if (source == eSourceInvalid)
    {
        send("SLIQSTN\n", 8);

        for (int i = 0; i < 10000; ++i)
        {
            if (source != eSourceInvalid)
            {
                break;
            }
            usleep(100);
        }
    }

    return source;
}

void OnkyoServer::sourceMessage(const std::string &data)
{
    if (data.compare("05") == 0)
    {
        source = ePC;
    }
    else if (data.compare("2B") == 0)
    {
        source = eNet;
    }
    else
    {
        source = eSourceInvalid;
    }
}

#ifdef DEBUG

void OnkyoServer::sendManual(std::string command)
{
    send(command.c_str(), command.size());
}

#endif
