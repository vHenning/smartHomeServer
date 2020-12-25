#include "DVDMode.h"
#include "../hmi/HMI.h"
#include "../benq/BenQServer.h"
#include "../onkyo/OnkyoServer.h"
#include "../video/VideoPlayer.h"

DVDMode::DVDMode()
{
    player = new VideoPlayer();

    fprintf(stderr, "Video Player started...\n");

//    OnkyoServer* onkyo = OnkyoServer::getInstance();

//    previousVolume = onkyo->getVolume();
//    previousSource = onkyo->getSource();
//    previousPower = onkyo->getPower();

//    onkyo->setPower(OnkyoServer::eOn);
//    onkyo->setSource(OnkyoServer::eBDDVD);
//    onkyo->setVolume(20);
//    //onkyo->setDim(eLowest);

//    BenQServer::getInstance()->setPowerState(BenQServer::ePowerOn);
//    BenQServer::getInstance()->setSource(BenQServer::eVGA);

    HMI::getInstance()->clearListeners();
    HMI::getInstance()->setListener(KEY_PLAYPAUSE, std::bind(&VideoPlayer::playPause, player));
    HMI::getInstance()->setListener(KEY_PREVIOUSSONG, std::bind(&VideoPlayer::previous, player));
    HMI::getInstance()->setListener(KEY_NEXTSONG, std::bind(&VideoPlayer::skip, player));
    HMI::getInstance()->setListener(KEY_UP, std::bind(&VideoPlayer::up, player));
    HMI::getInstance()->setListener(KEY_DOWN, std::bind(&VideoPlayer::down, player));
    HMI::getInstance()->setListener(KEY_LEFT, std::bind(&VideoPlayer::left, player));
    HMI::getInstance()->setListener(KEY_RIGHT, std::bind(&VideoPlayer::right, player));
    HMI::getInstance()->setListener(KEY_ENTER, std::bind(&VideoPlayer::enter, player));

}

DVDMode::~DVDMode()
{
//    OnkyoServer* onkyo = OnkyoServer::getInstance();
//    BenQServer* beamer = BenQServer::getInstance();

//    onkyo->setVolume(previousVolume);
//    onkyo->setSource(previousSource);
//    onkyo->setPower(previousPower);

//    beamer->setPowerState(BenQServer::ePowerStandby);

    delete player;
}
