#include "KodiMode.h"
#include "../ModeManager.h"
#include "../../video/KodiController.h"
#include "../../hmi/HMI.h"
#include "../../benq/BenQServer.h"
#include "../../onkyo/OnkyoServer.h"

KodiMode::KodiMode() : UserMode(eKodiMode)
{
    devices[eKodi]   = std::make_pair(1000, std::bind(&KodiMode::turnOnKodi,   this));
    devices[eHMI]    = std::make_pair(1000, std::bind(&KodiMode::turnOnHMI,    this));
    devices[eBeamer] = std::make_pair(1000, std::bind(&KodiMode::turnOnBeamer, this));
    devices[eStereo] = std::make_pair(1000, std::bind(&KodiMode::turnOnStereo, this));
}

void KodiMode::turnOnKodi()
{
    KodiController::getInstance()->start();
}

void KodiMode::turnOnHMI()
{
    HMI* hmi = HMI::getInstance();
    hmi->clearListeners();

    hmi->setBroadcastListener(std::bind(&KodiController::sendKey, KodiController::getInstance(), std::placeholders::_1));

    // We cannot add OnkyoServer::volumeUp/Down() methods directly as a listener
    // because we first have to check if this user mode is also in control of the stereo at the time of the key press.
    hmi->setListener(KEY_VOLUMEUP, std::bind(&KodiMode::volumeListener, this, true));
    hmi->setListener(KEY_VOLUMEDOWN, std::bind(&KodiMode::volumeListener, this, false));
}

void KodiMode::turnOnBeamer()
{
    BenQServer* beamer = BenQServer::getInstance();

    beamer->setPowerState(BenQServer::ePowerOn);
    beamer->setSource(BenQServer::eVGA);

    // Set output resolution to 1080p
    system("xrandr --newmode \"1920x1080\" 173.00 1920 2048 2248 2576 1080 1083 1088 1120 -hsync +vsync");
    system("xrandr --addmode VGA-0 1920x1080");
    system("xrandr --output VGA-0 --mode 1920x1080");
}

void KodiMode::turnOnStereo()
{
    OnkyoServer* stereo = OnkyoServer::getInstance();

    stereo->setPower(OnkyoServer::eOn);
    stereo->setVolume(20);
    stereo->setSource(OnkyoServer::eBDDVD);
}

void KodiMode::volumeListener(const bool &up)
{
    if (ModeManager::getInstance()->getActiveMode(eStereo) == getType())
    {
        up ? OnkyoServer::getInstance()->volumeUp() : OnkyoServer::getInstance()->volumeDown();
    }
}
