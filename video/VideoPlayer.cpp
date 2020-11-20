#include "VideoPlayer.h"

#include <vlc/vlc.h>
#include <unistd.h>

VideoPlayer::VideoPlayer(const MediaType &type, const std::string &path) :
    instance(libvlc_new(0, 0))
{
    if (!instance)
    {
        fprintf(stderr, "Error initializing VideoPlayer's instance of VLC player.\n");
    }

    libvlc_media_t *media;
    switch (type)
    {
    case eDVD:
        media = libvlc_media_new_path (instance, path.c_str());
        break;
    }

    if (!media)
    {
        fprintf(stderr, "Error initializing VideoPlayer's media.\n");
    }

    player = libvlc_media_player_new_from_media(media);

    if (!player)
    {
        fprintf(stderr, "Error initializing VideoPlayer's player instance.\n");
    }

    libvlc_media_release(media);

    libvlc_set_fullscreen(player, 1);

    libvlc_media_player_play(player);
}

VideoPlayer::~VideoPlayer()
{
    libvlc_media_player_stop(player);
    libvlc_media_player_release(player);
    libvlc_release(instance);
}

void VideoPlayer::playPause()
{
    libvlc_state_t state = libvlc_media_player_get_state(player);
    if (state == libvlc_Playing)
    {
        pause();
    }
    else
    {
        play();
    }
}

void VideoPlayer::play()
{
    libvlc_media_player_play(player);
}

void VideoPlayer::pause()
{
    libvlc_media_player_pause(player);
}

void VideoPlayer::up()
{
    libvlc_media_player_navigate(player, libvlc_navigate_up);
}

void VideoPlayer::down()
{
    libvlc_media_player_navigate(player, libvlc_navigate_down);
}

void VideoPlayer::left()
{
    libvlc_media_player_navigate(player, libvlc_navigate_left);
}

void VideoPlayer::right()
{
    libvlc_media_player_navigate(player, libvlc_navigate_right);
}

void VideoPlayer::enter()
{
    libvlc_media_player_navigate(player, libvlc_navigate_activate);
}

void VideoPlayer::skip()
{
    libvlc_media_player_next_chapter(player);
}

void VideoPlayer::previous()
{
    libvlc_media_player_previous_chapter(player);
}

void VideoPlayer::startFastForward()
{

}

void VideoPlayer::endFastForward()
{

}

void VideoPlayer::startFastRewind()
{

}

void VideoPlayer::endFastRewind()
{

}
