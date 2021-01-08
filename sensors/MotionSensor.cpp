#include "MotionSensor.h"

#include <thread>

MotionSensor::MotionSensor() : motionWasDetected(false), timerValid(false)
{
    const int PORT = 8003;
    socket = new boost::asio::ip::udp::socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), PORT));
    timer = new boost::asio::steady_timer(context);

    timer->expires_after(boost::asio::chrono::seconds(11));
    timer->async_wait(bind(&MotionSensor::handleTimerUp, this, std::placeholders::_1));

    keepAliveTimer = new boost::asio::steady_timer(context);
    keepAliveTimer->expires_after(boost::asio::chrono::seconds(1));
    keepAliveTimer->async_wait(bind(&MotionSensor::handleKeepAlive, this, std::placeholders::_1));

    receiveThread = std::thread(&MotionSensor::runReceive, this);
    timerThread = std::thread(&MotionSensor::runTimer, this);
}

bool MotionSensor::motionDetected()
{
    return motionWasDetected;
}

void MotionSensor::runReceive()
{
    bool running = true;
    while (running)
    {
        socket->wait(boost::asio::ip::udp::socket::wait_read);
        std::vector<char> bufferVector;
        bufferVector.resize(socket->available());
        boost::asio::ip::udp::endpoint from;
        socket->receive_from(boost::asio::buffer(bufferVector), from);

        uint32_t id = 0;
        memcpy(&id, bufferVector.data(), sizeof(uint32_t));

        if (id == 0x100)
        {
            socket->send_to(boost::asio::buffer(bufferVector), from);
            timer->cancel();
            timerValid = true;
            timer->expires_after(boost::asio::chrono::seconds(11));
            timer->async_wait(std::bind(&MotionSensor::handleTimerUp, this, std::placeholders::_1));

#ifdef DEBUG
            fprintf(stderr, "Motion Detected\n");
#endif
            if (!motionDetected())
            {
                motionWasDetected = true;

                for (size_t i = 0; i < motionHandlerFunctions.size(); ++i)
                {
                    motionHandlerFunctions[i]();
                }
            }
        }
    }
}

void MotionSensor::runTimer()
{
    context.run();
}

void MotionSensor::handleTimerUp(const boost::system::error_code &ec)
{
    if (ec.failed())
    {
        return;
    }
    if (timerValid)
    {
#ifdef DEBUG
        fprintf(stderr, "Motion stopped.\n");
#endif
        motionWasDetected = false;
        timerValid = false;
    }

    timer->cancel();
    timer->expires_after(boost::asio::chrono::seconds(11));
    timer->async_wait(std::bind(&MotionSensor::handleTimerUp, this, std::placeholders::_1));
}

void MotionSensor::handleKeepAlive(const boost::system::error_code &)
{
    keepAliveTimer->expires_after(boost::asio::chrono::seconds(1));
    keepAliveTimer->async_wait(std::bind(&MotionSensor::handleKeepAlive, this, std::placeholders::_1));
}
