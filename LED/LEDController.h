#ifndef LED_CONTROLER_H
#define LED_CONTROLLER_H

#include <stdint.h>
#include <string>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/io_service.hpp>

class LEDController
{
public:
    LEDController(const std::string &ip, const int& port = 8002);

    void setColor(const char &red, const char &green, const char &blue);

    void setDim(const double &dim);

    void setValues(const char &red, const char &green, const char &blue, const bool &raw = false);

    void setFilter(const bool &on);

    void setFilterValues(const double &capacitance, const double &impedance, const double &inductivity, bool resetInitialValues = true);

    void setFilterValues(const double &capacitance, const double &impedance, const double &inductivity,
                         const double &x1, const double &x2, const double &y1, const double &y2);


    struct ColorMessage
    {
        ColorMessage(const uint16_t &red, const uint16_t &green, const uint16_t &blue) : id(0x100), red(red), green(green), blue(blue) {}

        uint32_t id;
        uint16_t red;
        uint16_t green;
        uint16_t blue;
    };

    struct DimMessage
    {
        DimMessage(const float &dim) : id(0x101), dim(dim) {}

        uint32_t id;
        float dim;
    };

private:
    boost::asio::io_service service;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint partner;
};

#endif
