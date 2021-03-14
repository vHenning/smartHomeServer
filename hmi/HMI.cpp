#include "HMI.h"

#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "../userModes/ModeManager.h"

HMI::HMI() :
    running(true)
  , watchArrow(false)
{
    int keys = open("/dev/input/by-id/usb-123_COM_Smart_Control-if02-event-kbd", O_RDONLY);
    int specials = open("/dev/input/by-id/usb-123_COM_Smart_Control-if03-event-mouse", O_RDONLY);

    if (keys < 0 || specials < 0)
    {
        fprintf(stderr, "Opening input streams failed.\n");
        return;
    }

    // Detach remote from xinput
    int detachResult = system("xinput disable \"123 COM SmartControl Mouse\"");
    detachResult |= system("xinput disable \"123 COM Smart Control Consumer Control\"");
    detachResult |= system("xinput disable \"123 COM Smart Control System Control\"");
    detachResult |= system("xinput disable \"pointer:123 COM Smart Control\"");
    detachResult |= system("xinput disable \"123 COM Smart Control Consumer Control\"");

    if (detachResult != 0)
    {
        fprintf(stderr, "Error detaching remote from xinput\n");
    }

    keyThread = std::thread(&HMI::runKeyThread, this, keys);
    specialThread = std::thread(&HMI::runSpecialThread, this, specials);
}

HMI* HMI::getInstance()
{
    static HMI instance;
    return &instance;
}

HMI::~HMI()
{
    keyThread.join();
    specialThread.join();
}

void HMI::setListener(const int &key, const std::function<void (void)> listener)
{
    listeners[key] = listener;
}

void HMI::clearListeners()
{
    listeners.clear();
}

void HMI::callListeners(const int &key)
{
    std::unordered_map<int, std::function<void (void)>>::iterator it = listeners.find(key);
    if (it != listeners.end())
    {
        it->second();
    }
}

void HMI::runSpecialThread(int device)
{
    input_event event;

    while (running)
    {
        read(device, &event, sizeof(event));

        if (event.type == EV_REL || !event.code || event.code == 4)
        {
            continue;
        }

        if (event.code == KEY_HOMEPAGE && !watchArrow)
        {
            homePressed(event.value);
        }
        else if (watchArrow)
        {
            watchArrowPressed(event.code, event.value);
        }
        else if (event.value)
        {
            callListeners(event.code);
        }


//        fprintf(stderr, "Special: Value %d | code %d | type %d\n", event.value, event.code, event.type);
    }
}

void HMI::runKeyThread(int device)
{
    input_event event;

    while (running)
    {
        read(device, &event, sizeof(event));

        // Filter irrelevant keys
        if (event.type == EV_REL || !event.code || (event.value != 1 && event.value != 0))
        {
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

//        fprintf(stderr, "Key: Value %d | code %d | type %d\n", event.value, event.code, event.type);
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
//        UserModeManager::getInstance()->setMode(UserModeManager::eDVD);
        break;
    case KEY_HOMEPAGE:
//        UserModeManager::getInstance()->setMode(UserModeManager::eOff);
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
