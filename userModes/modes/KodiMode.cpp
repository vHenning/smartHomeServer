#include "KodiMode.h"
#include "../../video/KodiController.h"
#include "../../hmi/HMI.h"
#include "../../benq/BenQServer.h"

KodiMode::KodiMode() : UserMode(eKodiMode)
{
    devices[eKodi]   = std::make_pair(1000, std::bind(&KodiMode::turnOnKodi,   this));
    devices[eHMI]    = std::make_pair(1000, std::bind(&KodiMode::turnOnHMI,    this));
    devices[eBeamer] = std::make_pair(1000, std::bind(&KodiMode::turnOnBeamer, this));
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
}

void KodiMode::turnOnBeamer()
{
    BenQServer* beamer = BenQServer::getInstance();

    beamer->setPowerState(BenQServer::ePowerOn);
    beamer->setSource(BenQServer::eVGA);

    // TODO set resolution of beamer output?
}
