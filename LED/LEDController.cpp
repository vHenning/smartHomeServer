#include "LEDController.h"

#include <boost/asio/ip/udp.hpp>
#include <boost/asio.hpp>

LEDController::LEDController(const std::string &ip, const uint8_t &channel, std::function<void(boost::asio::ip::udp::endpoint, const unsigned char*, size_t)> commFunction) :
    channel(channel)
  , sendFunction(commFunction)
  , partner(new boost::asio::ip::udp::endpoint(boost::asio::ip::make_address_v4(ip), 8002))
{
}

void LEDController::sendBuffer(const unsigned char *buffer, const size_t &size)
{
    sendFunction(*partner, buffer, size);
}

void LEDController::setColor(const char &red, const char &green, const char &blue)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set color rgb %c %c %c\n", red, green, blue);
#endif

    uint16_t redBig = red << 8;
    uint16_t greenBig = green << 8;
    uint16_t blueBig = blue << 8;
    ColorMessage msg(channel, redBig, greenBig, blueBig);
    fprintf(stderr, "Color %02X %02X %02X\n", msg.red, msg.green, msg.blue);
    unsigned char buffer[sizeof(ColorMessage)];
    memcpy(buffer, &msg, sizeof(ColorMessage));
    sendBuffer(buffer, sizeof(ColorMessage));
}

void LEDController::setDim(const double &dim)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set dim %f\n", dim);
#endif

    DimMessage msg(channel, dim);
    unsigned char buffer[sizeof(DimMessage)];
    memcpy(buffer, &msg, sizeof(DimMessage));
    sendBuffer(buffer, sizeof(DimMessage));
}

void LEDController::setWhiteDim(const double &dim)
{
    WhiteDimMessage msg(channel, dim);
    unsigned char buffer[sizeof(WhiteDimMessage)];
    memcpy(buffer, &msg, sizeof(WhiteDimMessage));
    sendBuffer(buffer, sizeof(WhiteDimMessage));
}

void LEDController::setWhiteTemp(const double &temperature)
{
    WhiteTemperatureMessage msg(channel, temperature);
    unsigned char buffer[sizeof(WhiteTemperatureMessage)];
    memcpy(buffer, &msg, sizeof(WhiteTemperatureMessage));
    sendBuffer(buffer, sizeof(WhiteTemperatureMessage));
}

void LEDController::setWhiteMaxBrightness(const bool &maxWhite)
{
    WhiteMaxBrightnessMessage msg(channel, maxWhite);
    unsigned char buffer[sizeof(WhiteMaxBrightnessMessage)];
    memcpy(buffer, &msg, sizeof(WhiteMaxBrightnessMessage));
    sendBuffer(buffer, sizeof(WhiteMaxBrightnessMessage));
}

void LEDController::setValues(const char &red, const char &green, const char &blue, const bool &raw /*= false*/)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set values rgb %c %c %c %s\n", red, green, blue, raw ? "raw" : "not raw");
#endif

    // Shift arguments because ValueMessage takes 16 bit values
    uint16_t redShift = red << 8;
    uint16_t greenShift = green << 8;
    uint16_t blueShift = blue << 8;

    ValueMessage msg(channel, redShift, greenShift, blueShift, raw);
    unsigned char buffer[sizeof(ValueMessage)];
    memcpy(buffer, &msg, sizeof(ValueMessage));
    sendBuffer(buffer, sizeof(ValueMessage));
}

void LEDController::setFilter(const bool &on)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set filter %s\n", on ? "on" : "off");
#endif

    FilterMessage msg(channel, on);
    unsigned char buffer[sizeof(FilterMessage)];
    memcpy(buffer, &msg, sizeof(FilterMessage));
    sendBuffer(buffer, sizeof(FilterMessage));
}

void LEDController::setFilterValues(const double &capacitance, const double &impedance)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set filter values cap/imp/ind %f %f\n", capacitance, impedance);
#endif

    FilterValueMessage msg(channel, capacitance, impedance);
    unsigned char buffer[sizeof(FilterValueMessage)];
    memcpy(buffer, &msg, sizeof(FilterValueMessage));
    sendBuffer(buffer, sizeof(FilterValueMessage));
}

void LEDController::setFilterValues(const double &capacitance, const double &impedance,
                     const double &x1, const double &y1)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set filter values cap/imp x1/y1 %f %f %f %f\n", capacitance, impedance, x1, y1);
#endif

    FilterValueBufferMessage msg(channel, capacitance, impedance, x1, y1);
    unsigned char buffer[sizeof(FilterValueBufferMessage)];
    memcpy(buffer, &msg, sizeof(FilterValueBufferMessage));
    sendBuffer(buffer, sizeof(FilterValueBufferMessage));
}

void LEDController::turnOn(const bool &on)
{
    TurnOnOffMessage msg(channel, on);
    unsigned char buffer[sizeof(TurnOnOffMessage)];
    memcpy(buffer, &msg, sizeof(TurnOnOffMessage));
    sendBuffer(buffer, sizeof(TurnOnOffMessage));
}
