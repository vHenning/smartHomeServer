#include "KodiController.h"

#include <string>
#include <stdlib.h>
#include <linux/input-event-codes.h>

KodiController* KodiController::getInstance()
{
    static KodiController instance;
    return &instance;
}

KodiController::KodiController() : running(false) {}

bool KodiController::isActive() const
{
    return running;
}

void KodiController::start()
{
    if (!running)
    {
        system("kodi&");
        running = true;
    }
}

void KodiController::quit()
{
    if (running)
    {
        system("kodi-send --action=\"quit\"");
        running = false;
    }
}

void KodiController::sendKey(const int &key)
{
    if (running)
    {
        std::string keyName = "";

        switch (key)
        {
        case KEY_LEFT:
            keyName = "left";
            break;
        case KEY_RIGHT:
            keyName = "right";
            break;
        case KEY_UP:
            keyName = "up";
            break;
        case KEY_DOWN:
            keyName = "down";
            break;
        case KEY_ENTER:
            keyName = "enter";
            break;
        case KEY_BACK:
            keyName = "return";
            break;
        case KEY_ESC:
            keyName = "escape";
            break;
        case KEY_PLAYPAUSE:
            keyName = "play_pause";
            break;
        case KEY_FORWARD:
            keyName = "fastforward";
            break;
        case KEY_REWIND:
            keyName = "rewind";
            break;
        }

        if (!keyName.empty())
        {
            std::string command = "kodi-send --button=\"" + keyName + "\"";
            system(command.c_str());
        }
    }
}
