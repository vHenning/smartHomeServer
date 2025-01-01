#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <stdint.h>
#include <string>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/io_service.hpp>

class LEDController
{
public:
    LEDController(const std::string &ip, const uint8_t &channel, std::function<void(boost::asio::ip::udp::endpoint, const unsigned char*, size_t)> commFunction);

    void setColor(const char &red, const char &green, const char &blue);

    void setDim(const double &dim);

    void setWhiteDim(const double &dim);

    void setValues(const char &red, const char &green, const char &blue, const bool &raw = false);

    void setWhiteTemp(const double &temperature);

    void setFilter(const bool &on);

    void setFilterValues(const double &capacitance, const double &impedance);

    void setFilterValues(const double &capacitance, const double &impedance,
                         const double &x1, const double &y1);

    void turnOn(const bool &on);

private:
    void sendBuffer(const unsigned char *buffer, const size_t &size);

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
        DimMessage(const uint8_t &channel, const double &dim) :
            id(0x101)
          , channel(channel)
          , dim(dim)
        {}

        uint32_t id;
        uint8_t channel;
        double dim;
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

    struct __attribute__((__packed__)) FilterMessage
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

    struct __attribute__((__packed__)) FilterValueMessage
    {
        FilterValueMessage(const uint8_t &channel, const double &capacitance, const double &impedance) :
            id(0x104)
          , channel(channel)
          , capacitance(capacitance)
          , impedance(impedance)
        {}

        uint32_t id;
        uint8_t channel;
        double capacitance;
        double impedance;
    };

    struct __attribute__((__packed__)) FilterValueBufferMessage
    {
        FilterValueBufferMessage(const uint8_t &channel, const double &capacitance, const double &impedance,
                                 const float &x1, const float &y1) :
            id(0x105)
          , channel(channel)
          , capacitance(capacitance)
          , impedance(impedance)
          , x1(x1)
          , y1(y1)
        {}

        uint32_t id;
        uint8_t channel;
        double capacitance;
        double impedance;
        double x1;
        double y1;
    };

    struct __attribute__((__packed__)) WhiteDimMessage
    {
      WhiteDimMessage(const uint8_t &channel, const double &dim)
        : id(0x106)
        , channel(channel)
        , dim(dim)
      {}

      uint32_t id;
      uint8_t channel;
      double dim;
    };

    struct __attribute__((__packed__)) WhiteTemperatureMessage
    {
      WhiteTemperatureMessage(const uint8_t &channel, const double &temperature)
        : id(0x107)
        , channel(channel)
        , temperature(temperature)
      {}

      uint32_t id;
      uint8_t channel;
      double temperature;
    };

    struct __attribute__((__packed__)) TurnOnOffMessage
    {
      TurnOnOffMessage(const uint8_t &channel, const uint8_t &on)
        : id(0x108)
        , channel(channel)
        , on(on)
      {}

      uint32_t id;
      uint8_t channel;
      uint8_t on; // 1 = turn on, 0 = turn off
    };

    uint8_t channel;

    std::function<void(boost::asio::ip::udp::endpoint, const unsigned char*, size_t)> sendFunction;

    boost::asio::ip::udp::endpoint* partner;
};

#endif
