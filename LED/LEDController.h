#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <stdint.h>
#include <string>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/io_service.hpp>

class LEDController
{
public:
    LEDController(const std::string &ip, const uint8_t &channel = 0, const int& port = 8002);

    void setColor(const char &red, const char &green, const char &blue);

    void setDim(const double &dim);

    void setValues(const char &red, const char &green, const char &blue, const bool &raw = false);

    void setFilter(const bool &on);

    void setFilterValues(const double &capacitance, const double &impedance, const double &inductivity);

    void setFilterValues(const double &capacitance, const double &impedance, const double &inductivity,
                         const double &x1, const double &x2, const double &y1, const double &y2);


    struct __attribute__((__packed__)) ColorMessage
    {
        ColorMessage(const uint8_t &channel, const uint16_t &red, const uint16_t &green, const uint16_t &blue) :
            id(0x100)
          , channel(channel)
          , red(red)
          , green(green)
          , blue(blue)
        {}

        uint32_t id;
        uint8_t channel;
        uint16_t red;
        uint16_t green;
        uint16_t blue;
    };

    struct __attribute__((__packed__)) DimMessage
    {
        DimMessage(const uint8_t &channel, const float &dim) :
            id(0x101)
          , channel(channel)
          , dim(dim)
        {}

        uint32_t id;
        uint8_t channel;
        float dim;
    };

    struct __attribute__((__packed__)) ValueMessage
    {
        ValueMessage(const uint8_t &channel, const uint16_t &red, const uint16_t &green, const uint16_t &blue, const bool &raw = false) :
            id(0x102)
          , channel(channel)
          , red(red)
          , green(green)
          , blue(blue)
        {
            this->raw = static_cast<bool>(raw & 0x01);
        }

        uint32_t id;
        uint8_t channel;
        uint16_t red;
        uint16_t green;
        uint16_t blue;
        uint8_t raw;
    };

    struct FilterMessage
    {
        FilterMessage(const uint8_t &channel, const bool &filter) :
            id(0x103)
          , channel(channel)
        {
            this->filter = filter ? 0x01 : 0x00;
        }

        uint32_t id;
        uint8_t channel;
        uint8_t filter;
    };

    struct FilterValueMessage
    {
        FilterValueMessage(const uint8_t &channel, const float &capacitance, const float &impedance, const float &inductivity) :
            id(0x104)
          , channel(channel)
          , capacitance(capacitance)
          , impedance(impedance)
          , inductivity(inductivity)
        {}

        uint32_t id;
        uint8_t channel;
        float capacitance;
        float impedance;
        float inductivity;
    };

    struct FilterValueBufferMessage
    {
        FilterValueBufferMessage(const uint8_t &channel, const float &capacitance, const float &impedance, const float &inductivity,
                                 const float &x1, const float &x2, const float &y1, const float &y2) :
            id(0x105)
          , channel(channel)
          , capacitance(capacitance)
          , impedance(impedance)
          , inductivity(inductivity)
          , x1(x1)
          , x2(x2)
          , y1(y1)
          , y2(y2)
        {}

        uint32_t id;
        uint8_t channel;
        float capacitance;
        float impedance;
        float inductivity;
        float x1;
        float x2;
        float y1;
        float y2;
    };

private:
    uint8_t channel;

    boost::asio::io_service service;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint partner;
};

#endif
