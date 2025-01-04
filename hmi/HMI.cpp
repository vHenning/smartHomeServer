#include "HMI.h"

#include <string>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <ios>

#include "../userModes/ModeManager.h"

HMI::HMI() :
    running(true)
  , watchArrow(false)
{
    // Find input streams

    // Input streams are saved in /proc/bus/input/devices
    std::ifstream devices;
    devices.open("/proc/bus/input/devices");

    bool searchForHandler = false;

    std::string line = "";
    if (devices.is_open())
    {
        while (true)
        {
            std::getline(devices, line);

            // If end of file
            std::ios_base::iostate state = devices.rdstate();
            if ((state & std::ifstream::eofbit) != 0)
            {
                break;
            }

            if (line.find("Name=\"123") != std::string::npos)
            {
                searchForHandler = true;
            }
            else if (line.find("Name=") != std::string::npos)
            {
                searchForHandler = false;
            }

            if (searchForHandler && line.find("Handlers=") != std::string::npos)
            {
                size_t position = line.find("event");
                if (position != std::string::npos)
                {
                    // Position of the number (e.g. 23 for event23)
                    position += 5;

                    Stream* stream = new Stream();

                    size_t spacePosition = line.find_first_of(" ", position);
                    stream->eventNumber = line.substr(position, spacePosition - position);

                    streams.push_back(stream);
                }
            }
        }

        devices.close();
    }
    else
    {
        fprintf(stderr, "Could not open /proc/bus/input/devices!\n");
    }

    // Open input streams
    for (size_t i = 0; i < streams.size(); ++i)
    {
        std::string filename = "/dev/input/event" + streams[i]->eventNumber;
        int file = open(filename.c_str(), O_RDONLY);
        if (file < 0)
        {
            fprintf(stderr, "Error opening %s\n", streams[i]->eventNumber.c_str());
            continue;
        }

        // Start thread for this input
        streams[i]->thread = std::thread(&HMI::runThread, this, file);
    }

    // Disconnect HMI from xinput

    FILE* output = popen("xinput", "r");
    const size_t BUFFER_SIZE = 256;
    char xinputLine[BUFFER_SIZE];

    while (fgets(xinputLine, BUFFER_SIZE, output) != 0)
    {
        std::string stringLine(xinputLine);
        if (stringLine.find("123") != std::string::npos)
        {
            size_t idPosition = stringLine.find("id=");
            idPosition += 3;

            size_t tabPosition   = stringLine.find("\t", idPosition);
            size_t spacePosition = stringLine.find(" ", idPosition);

            std::string id = stringLine.substr(idPosition, std::min(tabPosition, spacePosition) - idPosition);

            // Disable devices from xinput
            std::string disableCommand = "xinput disable " + id;
            int success = system(disableCommand.c_str());
            if (success != 0)
            {
                fprintf(stderr, "Error detatching input %s from xinput\n", id.c_str());
            }
        }
    }
    pclose(output);
}

HMI* HMI::getInstance()
{
    static HMI instance;
    return &instance;
}

HMI::~HMI()
{
    running = false;

    for (size_t i = 0; i < streams.size(); ++i)
    {
        streams[i]->thread.join();
    }
}

void HMI::setListener(const int &key, const std::function<void (void)> listener)
{
    listeners[key] = listener;
}

void HMI::setBroadcastListener(const std::function<void (int)>& listener)
{
    broadcastListeners.push_back(listener);
}

void HMI::clearListeners()
{
    listeners.clear();
    broadcastListeners.clear();
}

void HMI::callListeners(const int &key)
{
    std::unordered_map<int, std::function<void (void)>>::iterator it = listeners.find(key);
    if (it != listeners.end())
    {
        it->second();
    }

    for (size_t i = 0; i < broadcastListeners.size(); ++i)
    {
        broadcastListeners[i](key);
    }
}

void HMI::runThread(int device)
{
    input_event event;

    while (running)
    {
        read(device, &event, sizeof(event));

        // Filter irrelevant keys
        if (event.type == EV_REL || !event.code || event.code == 4)
        {
            continue;
        }

        if (event.code == KEY_HOMEPAGE && !watchArrow)
        {
            homePressed(event.value);
            continue;
        }

        if (watchArrow)
        {
            watchArrowPressed(event.code, event.value);
        }
        else if (event.value)
        {
            callListeners(event.code);
        }
    }
}

void HMI::homePressed(bool pressed)
{
    if (pressed)
    {
        watchArrow = true;
        // set up timer
    }
}

void HMI::watchArrowPressed(int key, bool pressed)
{
    if (!pressed)
    {
        return;
    }

    bool success = true;

    switch (key)
    {
    case KEY_UP:
        ModeManager::getInstance()->addMode(UserMode::eKodiMode);
        break;
    case KEY_HOMEPAGE:
        // Remove all modes that are reactivated if their button is pressed
        if (ModeManager::getInstance()->isActive(UserMode::eKodiMode))
        {
            ModeManager::getInstance()->removeMode(UserMode::eKodiMode);
        }
        break;
    case KEY_BACK:
        break;
    case KEY_DOWN:
        if (ModeManager::getInstance()->isActive(UserMode::eReadingMode))
        {
            ModeManager::getInstance()->removeMode(UserMode::eReadingMode);
        }
        else
        {
            ModeManager::getInstance()->addMode(UserMode::eReadingMode);
        }
        break;
    default:
        success = false;
    }

    if (success)
    {
        watchArrow = false;
        // stop timer
    }
}
