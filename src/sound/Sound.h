#ifndef SOUND_H
#define SOUND_H

#include <string>

class Sound
{
public:
    static Sound* fromWAV(const std::string &path);
    static Sound* fromWAVString(const std::string &data);

    ~Sound();

    Sound();

    int32_t dataSize;
    int16_t bitsPerSample;
    int32_t bytesPerSecond;
    int32_t sampleRate;
    int16_t channels;

    char* data;
};

#endif // SOUND_H
