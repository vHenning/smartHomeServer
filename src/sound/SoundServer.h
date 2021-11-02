#ifndef SOUND_SERVER_H
#define SOUND_SERVER_H

#include "Sound.h"

class SoundServer
{
public:
    static int play(const Sound &sound);
};

#endif // SOUND_SERVER_H
