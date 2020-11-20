#include "PCSensor.h"

#include <boost/bind/bind.hpp>

PCSensor::PCSensor() : pcIsOn(false), timerValid(false)
{
    const int PORT = 8001;
    socket = new boost::asio::ip::udp::socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), PORT));
    timer = new boost::asio::steady_timer(context);

    timer->expires_after(boost::asio::chrono::seconds(11));
    timer->async_wait(boost::bind(&PCSensor::handleTimerUp, this, boost::placeholders::_1));

    keepAliveTimer = new boost::asio::steady_timer(context);
    keepAliveTimer->expires_after(boost::asio::chrono::seconds(1));
    keepAliveTimer->async_wait(boost::bind(&PCSensor::handleKeepAlive, this, boost::placeholders::_1));

    receiveThread = std::thread(&PCSensor::runReceive, this);
    timerThread = std::thread(&PCSensor::runTimer, this);
}

bool PCSensor::isOn()
{
    return pcIsOn;
}

void PCSensor::runReceive()
{
    bool running = true;
    while (running)
    {
        socket->wait(boost::asio::ip::udp::socket::wait_read);
        std::vector<char> bufferVector;
        bufferVector.resize(socket->available());
        boost::asio::ip::udp::endpoint from;
        socket->receive_from(boost::asio::buffer(bufferVector), from);

        socket->send_to(boost::asio::buffer(bufferVector), from);

        // TODO: Check message format
        fprintf(stderr, "PC on\n");

        timer->cancel();
        timerValid = true;
        timer->expires_after(boost::asio::chrono::seconds(11));
        timer->async_wait(boost::bind(&PCSensor::handleTimerUp, this, boost::placeholders::_1));

        if (!pcIsOn)
        {
            pcIsOn = true;

            for (size_t i = 0; i < onHandlerFunctions.size(); ++i)
            {
                onHandlerFunctions[i]();
            }
        }
    }
}

void PCSensor::runTimer()
{
    context.run();
}

void PCSensor::handleTimerUp(const boost::system::error_code &ec)
{
    if (ec.failed())
    {
        return;
    }
    if (timerValid)
    {
        fprintf(stderr, "PC off\n");
        pcIsOn = false;
        timerValid = false;

        for (size_t i = 0; i < offHandlerFunctions.size(); ++i)
        {
            offHandlerFunctions[i]();
        }
    }

    timer->cancel();
    timer->expires_after(boost::asio::chrono::seconds(11));
    timer->async_wait(boost::bind(&PCSensor::handleTimerUp, this, boost::placeholders::_1));
}

void PCSensor::handleKeepAlive(const boost::system::error_code &)
{
    keepAliveTimer->expires_after(boost::asio::chrono::seconds(1));
    keepAliveTimer->async_wait(boost::bind(&PCSensor::handleKeepAlive, this, boost::placeholders::_1));
}
