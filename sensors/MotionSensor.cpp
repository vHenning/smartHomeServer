#include "MotionSensor.h"

#include <thread>

MotionSensor::MotionSensor()
{
    const int PORT = 8003;
    socket = new boost::asio::ip::udp::socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), PORT));

    keepAliveTimer = new boost::asio::steady_timer(context);
    keepAliveTimer->expires_after(boost::asio::chrono::seconds(1));
    keepAliveTimer->async_wait(bind(&MotionSensor::handleKeepAlive, this, std::placeholders::_1));

    receiveThread = std::thread(&MotionSensor::runReceive, this);
    timerThread = std::thread(&MotionSensor::runTimer, this);
}

bool MotionSensor::motionDetected(const uint32_t &unitID)
{
    UnitMap::iterator it = units.find(unitID);

    if (it == units.end())
    {
        return false;
    }
    else
    {
        return it->second.motionDetected();
    }
}

void MotionSensor::addMotionHandler(const uint32_t &unitID, std::function<void ()> handler)
{
    UnitMap::iterator it = units.find(unitID);

    if (it == units.end())
    {
        units.insert(std::make_pair(unitID, Unit(&context)));
        it = units.find(unitID);
    }

    it->second.motionHandlerFunctions.push_back(handler);
}

void MotionSensor::addMotionStopHandler(const uint32_t &unitID, std::function<void ()> handler, unsigned int delaySeconds)
{
    UnitMap::iterator it = units.find(unitID);

    if (it == units.end())
    {
        units.insert(std::make_pair(unitID, Unit(&context)));
        it = units.find(unitID);
    }

    if (delaySeconds < 10)
    {
        delaySeconds = 10;
    }

    // TODO: make stopMotionHandlers addable if off timer is already running:

    it->second.offHandlers.push_back(Unit::OffHandler(delaySeconds, handler, &context));
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
        size_t size = socket->receive_from(boost::asio::buffer(bufferVector), from);

        uint32_t id = 0;
        memcpy(&id, bufferVector.data(), sizeof(uint32_t));

        if (id == 0x100 && size >= 8)
        {
            socket->send_to(boost::asio::buffer(bufferVector), from);

            uint32_t unitID = 0;
            memcpy(&unitID, &bufferVector[4], sizeof(uint32_t));

            UnitMap::iterator it = units.find(unitID);
            if (it == units.end())
            {
                units.insert(std::make_pair(unitID, Unit(&context)));
                it = units.find((unitID));
            }

            it->second.timerValid = true;
            boost::asio::steady_timer* timer = it->second.timer;

            timer->cancel();
            timer->expires_after(boost::asio::chrono::seconds(11));
            timer->async_wait(std::bind(&MotionSensor::Unit::handleTimerUp, &it->second, std::placeholders::_1));

            for (size_t i = 0; i < it->second.offHandlers.size(); ++i)
            {
                it->second.offHandlers[i].timer.cancel();
                it->second.offHandlers[i].timer.expires_after(boost::asio::chrono::seconds(it->second.offHandlers[i].delayTime));
                it->second.offHandlers[i].timer.async_wait(std::bind(&Unit::OffHandler::handleTimeout, &it->second.offHandlers[i], std::placeholders::_1));
            }

#ifdef DEBUG
            fprintf(stderr, "Motion Detected\n");
#endif
            if (!it->second.motionDetected())
            {
                it->second.motionWasDetected = true;

                for (size_t i = 0; i < it->second.motionHandlerFunctions.size(); ++i)
                {
                    it->second.motionHandlerFunctions[i]();
                }
            }
        }
    }
}

void MotionSensor::runTimer()
{
    context.run();
}

void MotionSensor::Unit::handleTimerUp(const boost::system::error_code &ec)
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
    timer->async_wait(std::bind(&MotionSensor::Unit::handleTimerUp, this, std::placeholders::_1));
}

void MotionSensor::handleKeepAlive(const boost::system::error_code &)
{
    keepAliveTimer->expires_after(boost::asio::chrono::seconds(1));
    keepAliveTimer->async_wait(std::bind(&MotionSensor::handleKeepAlive, this, std::placeholders::_1));
}

MotionSensor::Unit::Unit(boost::asio::io_context *context)
    : motionWasDetected(false)
    , timerValid(false)
{
    timer = new boost::asio::steady_timer(*context);
    timer->expires_after(boost::asio::chrono::seconds(11));
}

bool MotionSensor::Unit::motionDetected()
{
    return motionWasDetected;
}
