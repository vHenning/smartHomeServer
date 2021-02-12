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

    /**
     * Is there motion right now?
     * @param unitID - Motion detector unit to check
     * @return - True if motion was detected at given unit, false otherwise
     */
    bool motionDetected(const uint32_t &unitID);

    /**
     * Add a function that shall be called if motion is detected at given sensor unit
     * @param unitID - The sensor unit that shall trigger function execution if it detects motion
     * @param handler - The function that shall be called if motion is detected
     */
    void addMotionHandler(const uint32_t &unitID, std::function<void (void)> handler);

    /**
     * Add a function that shall be called if motion stops at given sensor unit
     * @param unitID - The sensor unit that shall trigger function execution if motion stops
     * @param handler - The function that shall be called if motion is detected
     * @param delaySeconds - Number of seconds function execution shall be delayed after motion has stopped.
     *                       Minimum is 10 seconds. If motion is detected again before the delay time has passed, the handler is not called.
     */
    void addMotionStopHandler(const uint32_t &unitID, std::function<void (void)> handler, unsigned int delaySeconds = 10);

private:

    /**
     * This represents a motion detector unit
     */
    struct Unit
    {
        /**
         * Constructor.
         * @param context - Needed for the valid timer
         */
        Unit(boost::asio::io_context* context);

        /**
         * Check if motion is being detected right now at this unit
         * @return - True if motion is being detected, false otherwise
         */
        bool motionDetected();

        /**
         * Called by timer if it expires. Sets motionWasDetected false.
         * @param ec - Timer error code
         */
        void handleTimerUp(const boost::system::error_code &ec);

        /**
         * True if motion is being detected right now
         */
        bool motionWasDetected;

        /**
         * All handler functions that are called if motion is detected
         */
        std::vector<std::function<void (void)>> motionHandlerFunctions;

        struct OffHandler
        {
            OffHandler(const int &delay, const std::function<void (void)> &handler, boost::asio::io_context* context)
                : delayTime(delay)
                , handlerFunction(handler)
                , timer(*context)
            {
                timer.expires_after(boost::asio::chrono::seconds(delay));
            }

            int delayTime;
            std::function<void (void)> handlerFunction;
            boost::asio::steady_timer timer;

            void handleTimeout(const boost::system::error_code &ec)
            {
                if (!ec)
                {
                    handlerFunction();
                }
            }
        };

        std::vector<OffHandler> offHandlers;

        /**
         * Timer that expires after a given amount of time, sets motionWasDetected false when expired.
         */
        boost::asio::steady_timer* timer;

        /**
         * True if the timer is currently running
         */
        bool timerValid;
    };

    /**
     * uint32_t represents the unit id
     */
    typedef std::map<uint32_t, Unit> UnitMap;
    UnitMap units;

    void handleKeepAlive(const boost::system::error_code &);

    std::thread receiveThread;
    std::thread timerThread;

    void runReceive();
    void runTimer();

    boost::asio::ip::udp::socket* socket;
    boost::asio::io_context context;

    boost::asio::steady_timer* keepAliveTimer;
};

#endif // MOTIONSENSOR_H
