#include "LEDController.h"

#include <boost/asio/ip/udp.hpp>
#include <boost/asio.hpp>

LEDController::LEDController(const std::string &ip, const int &port) :
    socket(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
  , partner(boost::asio::ip::make_address_v4(ip), port)
{
}

void LEDController::setColor(const char &red, const char &green, const char &blue)
{
    uint16_t redBig = red << 8;
    uint16_t greenBig = green << 8;
    uint16_t blueBig = blue << 8;
    ColorMessage msg(redBig, greenBig, blueBig);
    fprintf(stderr, "Color %02X %02X %02X\n", msg.red, msg.green, msg.blue);
    unsigned char buffer[sizeof(ColorMessage)];
    memcpy(buffer, &msg, sizeof(ColorMessage));
    socket.send_to(boost::asio::buffer(buffer, sizeof(ColorMessage)), partner);
}

void LEDController::setDim(const double &dim)
{
    DimMessage msg(dim);
    unsigned char buffer[sizeof(DimMessage)];
    memcpy(buffer, &msg, sizeof(DimMessage));
    socket.send_to(boost::asio::buffer(buffer, sizeof(DimMessage)), partner);
}
