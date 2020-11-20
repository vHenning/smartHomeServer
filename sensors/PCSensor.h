#ifndef PCSENSOR_H
#define PCSENSOR_H

#include <thread>
#include <functional>

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/steady_timer.hpp>

#include <boost/function.hpp>

class PCSensor
{
public:    
    PCSensor();

    bool isOn();

    std::vector<boost::function<void (void)>> onHandlerFunctions;
    std::vector<boost::function<void (void)>> offHandlerFunctions;

private:

    bool pcIsOn;
    bool timerValid;

    void handleTimerUp(const boost::system::error_code& ec);
    void handleKeepAlive(const boost::system::error_code &);

    std::thread receiveThread;
    std::thread timerThread;

    void runReceive();
    void runTimer();

    boost::asio::ip::udp::socket* socket;
    boost::asio::io_context context;

    boost::asio::steady_timer* timer;
    boost::asio::steady_timer* keepAliveTimer;
};

#endif
