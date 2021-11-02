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
        std::string actionName = "";

        switch (key)
        {
        case KEY_LEFT:
            actionName = "Left";
            break;
        case KEY_RIGHT:
            actionName = "Right";
            break;
        case KEY_UP:
            actionName = "Up";
            break;
        case KEY_DOWN:
            actionName = "Down";
            break;
        case KEY_ENTER:
        case BTN_MOUSE:
            actionName = "Select";
            break;
        case KEY_BACK:
        case KEY_ESC:
            actionName = "Back";
            break;
        case KEY_PLAYPAUSE:
            actionName = "PlayPause";
            break;
        case KEY_NEXTSONG:
            actionName = "FastForward";
            break;
        case KEY_PREVIOUSSONG:
            actionName = "Rewind";
            break;
        }

        if (!actionName.empty())
        {
            std::string command = "kodi-send --action=\"Action(" + actionName + ")\"";
            system(command.c_str());
        }
    }
}
