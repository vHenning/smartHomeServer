#include "LEDController.h"

#include <boost/asio/ip/udp.hpp>
#include <boost/asio.hpp>

LEDController::LEDController(const std::string &ip, const uint8_t &channel, const int &port) :
    channel(channel)
  , socket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
  , partner(boost::asio::ip::make_address_v4(ip), port)
{
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
    socket.send_to(boost::asio::buffer(buffer, sizeof(ColorMessage)), partner);
}

void LEDController::setDim(const double &dim)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set dim %f\n", dim);
#endif

    DimMessage msg(channel, dim);
    unsigned char buffer[sizeof(DimMessage)];
    memcpy(buffer, &msg, sizeof(DimMessage));
    socket.send_to(boost::asio::buffer(buffer, sizeof(DimMessage)), partner);
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
    socket.send_to(boost::asio::buffer(buffer, sizeof(ValueMessage)), partner);
}

void LEDController::setFilter(const bool &on)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set filter %s\n", on ? "on" : "off");
#endif

    FilterMessage msg(channel, on);
    unsigned char buffer[sizeof(FilterMessage)];
    memcpy(buffer, &msg, sizeof(FilterMessage));
    socket.send_to(boost::asio::buffer(buffer, sizeof(FilterMessage)), partner);
}

void LEDController::setFilterValues(const double &capacitance, const double &impedance, const double &inductivity)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set filter values cap/imp/ind %f %f %f\n", capacitance, impedance, inductivity);
#endif

    FilterValueMessage msg(channel, capacitance, impedance, inductivity);
    unsigned char buffer[sizeof(FilterValueMessage)];
    memcpy(buffer, &msg, sizeof(FilterValueMessage));
    socket.send_to(boost::asio::buffer(buffer, sizeof(FilterValueMessage)), partner);
}

void LEDController::setFilterValues(const double &capacitance, const double &impedance, const double &inductivity,
                     const double &x1, const double &x2, const double &y1, const double &y2)
{
#ifdef DEBUG
    fprintf(stderr, "LED Set filter values cap/imp/ind x1/x2/y1/y2 %f %f %f %f %f %f %f\n", capacitance, impedance, inductivity, x1, x2, y1, y2);
#endif

    FilterValueBufferMessage msg(channel, capacitance, impedance, inductivity, x1, x2, y1, y2);
    unsigned char buffer[sizeof(FilterValueBufferMessage)];
    memcpy(buffer, &msg, sizeof(FilterValueBufferMessage));
    socket.send_to(boost::asio::buffer(buffer, sizeof(FilterValueBufferMessage)), partner);
}
