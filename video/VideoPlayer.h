#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <string>
#include <vlc/vlc.h>

class VideoPlayer
{
    enum MediaType
    {
        eDVD
    };

public:
    VideoPlayer(const MediaType &type = eDVD, const std::string &path = "/dev/dvd");

    ~VideoPlayer();

    void playPause();
    void play();
    void pause();

    void up();
    void down();
    void left();
    void right();
    void enter();

    void skip();
    void previous();

    void startFastForward();
    void endFastForward();
    void startFastRewind();
    void endFastRewind();

private:


    MediaType type;

    libvlc_media_player_t* player;
    libvlc_instance_t* instance;
};

#endif // VIDEO_PLAYER_H
