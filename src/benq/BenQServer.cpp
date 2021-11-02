#include "BenQServer.h"

#include <boost/asio/write.hpp>

//#define DEBUG

BenQServer::BenQServer() :
    newReceived(false),
    waiting(false),
    running(true)
{
    port = new boost::asio::serial_port(context, "/dev/ttyUSB0");

    boost::asio::serial_port_base::baud_rate baudrate(9600);
    boost::asio::serial_port_base::parity parity(boost::asio::serial_port_base::parity::none);
    boost::asio::serial_port_base::stop_bits stopBits(boost::asio::serial_port_base::stop_bits::one);
    boost::asio::serial_port_base::flow_control flowControl(boost::asio::serial_port_base::flow_control::none);
    port->set_option(baudrate);
    port->set_option(boost::asio::serial_port_base::character_size(8));
    port->set_option(parity);
    port->set_option(stopBits);
    port->set_option(flowControl);

    receiveThread = std::thread(&BenQServer::runReceive, this);
}

BenQServer* BenQServer::getInstance()
{
    static BenQServer instance;
    return &instance;
}

BenQServer::~BenQServer()
{
    running = false;
    port->cancel();
    port->close();
    receiveThread.join();
}

BenQServer::PowerState BenQServer::getPowerState()
{
    std::string received = sendCommand("POW=?");

    if (received == "POW=OFF")
    {
        return ePowerStandby;
    }
    else if (received == "POW=ON")
    {
        return ePowerOn;
    }
    fprintf(stderr, "BenQ: GetPowerState() failed.\n");
    return ePowerInvalid;
}

BenQServer::PowerState BenQServer::setPowerState(PowerState value)
{
    std::string command;

    switch (value)
    {
    case ePowerInvalid:
        fprintf(stderr, "Power state cannot be set invalid.\n");
        return ePowerInvalid;
        break;
    case ePowerOn:
        command = "POW=ON";
        break;
    case ePowerStandby:
        command = "POW=OFF";
        break;
    }
    std::string received = sendCommand(command);

    if (received == "Block item\r\n")
    {
        return value;
    }
    if (received == "POW=OFF")
    {
        return ePowerStandby;
    }
    else if (received == "POW=ON")
    {
        return ePowerOn;
    }
    else
    {
        return ePowerInvalid;
    }
}

BenQServer::Source BenQServer::getSource()
{
    std::string received = sendCommand("SOUR=?");

    if (received == "SOUR=RGB")
    {
        return eVGA;
    }
    else if (received == "SOUR=HDMI")
    {
        return eHDMI1;
    }
    else if (received == "SOUR=HDMI2")
    {
        return eHDMI2;
    }

    fprintf(stderr, "BenQ: GetSource() failed.\n");
    return eSourceInvalid;
}

BenQServer::Source BenQServer::setSource(Source value)
{
    std::string command;

    switch (value)
    {
    case eSourceInvalid:
        fprintf(stderr, "Source cannot be set invalid.\n");
        return eSourceInvalid;
        break;
    case eVGA:
        command = "SOUR=RGB";
        break;
    case eHDMI1:
        command = "SOUR=HDMI";
        break;
    case eHDMI2:
        command = "SOUR=HDMI2";
        break;
    }
    std::string received = sendCommand(command);

    if (received == "Block item\r\n")
    {
        return value;
    }
    if (received == "SOUR=RGB")
    {
        return eVGA;
    }
    else if (received == "SOUR=HDMI")
    {
        return eHDMI1;
    }
    else if (received == "SOUR=HDMI2")
    {
        return eHDMI2;
    }

    fprintf(stderr, "BenQ: SetSource() failed.\n");
    return eSourceInvalid;
}

void BenQServer::runReceive()
{
    while (port == 0 || !port->is_open())
    {
        usleep(100);
    }
    fprintf(stderr, "BenQ Serial Port opened.\n");

    while(running)
    {
        while (newReceived)
        {
#ifdef DEBUG
            fprintf(stderr, "[RECEIVER] waiting\n");
#endif
            std::unique_lock<std::mutex> uniqueLock(conditionalMutex);
            waiting = true;
            waitReceive.wait(uniqueLock);
            waiting = false;
        }
#ifdef DEBUG
        fprintf(stderr, "[RECEIVER] wait ended, waiting for receive\n");
#endif

        std::vector<char> buffer;
        buffer.resize(1);
        boost::system::error_code ec;
        port->read_some(boost::asio::buffer(buffer), ec);
        if (ec.failed())
        {
            fprintf(stderr, "Error receiving: %s\n", ec.message().c_str());
        }
#ifdef DEBUG
        if (*buffer.data() == '\r')
        {
            fprintf(stderr, "Received CR\n");
        }
        else if (*buffer.data() == '\n')
        {
            fprintf(stderr, "Received LF\n");
        }
        else if (*buffer.data() == 0)
        {
            fprintf(stderr, "Received 0\n");
        }
        else
        {
            fprintf(stderr, "Received %c (%d)\n", *buffer.data(), *buffer.data());
        }
#endif
        receivedMutex.lock();
        received = *buffer.data();
        receivedMutex.unlock();
        newReceived = true;
    }
}

char BenQServer::getChar()
{
#ifdef DEBUG
    fprintf(stderr, "GETCHAR\n");
#endif
    receivedMutex.lock();
    char sendback = received;
    receivedMutex.unlock();
    if (newReceived)
    {
        newReceived = false;
        usleep(10);
    }
    waitReceive.notify_all();
    return sendback;
}

std::string BenQServer::sendCommand(const std::string& command)
{
    if (!port->is_open())
    {
        fprintf(stderr, "Port is not open.\n");
        return "";
    }
    // Clear all eventual data in buffer
    while (newReceived)
    {
        getChar();
#ifdef DEBUG
        char old = getChar();
        fprintf(stderr, "Clearing old data: %c\n", old);
#endif
        for (int i = 0; i < 1000; ++i)
        {
            if (newReceived)
            {
                break;
            }
            usleep(10);
        }
    }

    // Send first CR and see if we get a '>' back
    // (This means "OK, I'm listening")
    // If not, it means this was the terminating CR of a previous (probably unsuccessful) command.
    std::vector<char> bufferVector;
    bufferVector.push_back('\r');
    size_t sent = port->write_some(boost::asio::buffer(bufferVector));
    if (sent != bufferVector.size())
    {
        fprintf(stderr, "Sent less than intended (sent: %lu, intended: %lu)\n", sent, bufferVector.size());
    }
    waitForReceive(10);
    if (newReceived)
    {
        char received = getChar();

        // If it is not '>', it is probably an error message from a failed earlier command.
        if (received != '>')
        {
#ifdef DEBUG
            fprintf(stderr, "Did not receive '>' but %c\n", received);
#endif
            // So read the error message
            waitForReceive(10);
            while (newReceived)
            {
                getChar();
                waitForReceive(10);
            }

            // Then send CR again to start the new command (we are expecting a '>' as answer again)
            size_t sent = port->write_some(boost::asio::buffer(bufferVector));
            if (sent != bufferVector.size())
            {
                fprintf(stderr, "Sent less than intended (sent: %lu, intended: %lu)\n", sent, bufferVector.size());
            }
            waitForReceive(10);

            // If we do not receive a '>', dont try any longer and return false
            if (!newReceived || getChar() != '>')
            {
#ifdef DEBUG
                fprintf(stderr, "Did not receive '>' after resending\n");
#endif
                return "";
            }
            else
            {
                return sendActualCommand(command);
            }
        }
        else
        {
            return sendActualCommand(command);
        }
    }

    // if we did not receive anything, try sending CR again.
    sent = port->write_some(boost::asio::buffer(bufferVector));
    if (sent != bufferVector.size())
    {
        fprintf(stderr, "Sent less than intended (sent: %lu, intended: %lu)\n", sent, bufferVector.size());
    }
    waitForReceive(10);

    if (newReceived && getChar() == '>')
    {
        return sendActualCommand(command);
    }
    else
    {
#ifdef DEBUG
        fprintf(stderr, "No '>' after resending CR. newReceived is %s\n", newReceived ? "true" : "false");
#endif
        getChar();
        return "";
    }
}

std::string BenQServer::sendActualCommand(const std::string& command)
{
    // If we are at this point, '>' has been received so the beamer is ready to receive our command

    std::vector<char> bufferVector;
    std::string sendCommand = "*" + command + "#\r";
#ifdef DEBUG
    fprintf(stderr, "Sending %s\n", sendCommand.c_str());
#endif
    bufferVector.resize(sendCommand.length());
    memcpy(bufferVector.data(), sendCommand.c_str(), sendCommand.length());
    size_t sent = port->write_some(boost::asio::buffer(bufferVector));
    if (sent != bufferVector.size())
    {
        fprintf(stderr, "Sent less than intended (sent: %lu, intended: %lu)\n", sent, bufferVector.size());
    }

    // Receive the EOF ('\0’) of the previously sent '>'
    std::string received = receivePacket();
#ifdef DEBUG
    fprintf (stderr, "Received first: %s\n", received.c_str());
#endif
    // Receive the echo of this command
    received = receivePacket();
#ifdef DEBUG
    fprintf (stderr, "Received second: %s\n", received.c_str());
#endif

    // Receive the answer of this command
    std::string answer = receivePacket();
#ifdef DEBUG
    fprintf (stderr, "Received third: %s\n", answer.c_str());
#endif


    size_t asterisk = answer.find_first_of('*');
    size_t pound = answer.find_first_of('#');

    if (asterisk != std::string::npos && pound != std::string::npos && pound > asterisk)
    {
        return answer.substr(asterisk + 1, pound - asterisk - 1);
    }
    else
    {
        return answer;
    }
}

void BenQServer::waitForReceive(const int& millis)
{
    int condition = millis * 100;
    for (int i = 0; i < condition; ++i)
    {
        if (newReceived)
        {
            return;
        }
        usleep(10);
    }
}

std::string BenQServer::receivePacket()
{
    std::string message = "";
    int timeout = 1000;
    waitForReceive(timeout);
    while (true)
    {
        if (newReceived)
        {
            char received = getChar();
            if (received == 0)
            {
                return message;
            }
            else
            {
                message += received;
            }
        }
        else
        {
            return "";
        }
        waitForReceive(timeout);
    }

}
