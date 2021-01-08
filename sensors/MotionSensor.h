#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <functional>
#include <thread>

class MotionSensor
{
public:
    MotionSensor();

    bool motionDetected();

    std::vector<std::function<void (void)>> motionHandlerFunctions;

private:

    bool motionWasDetected;
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

#endif // MOTIONSENSOR_H
