#include "Sound.h"

#include <cstdio>
#include <cstring>

Sound* Sound::fromWAV(const std::string &path)
{
    FILE * file = fopen(path.c_str(), "r");

    if (file == 0)
    {
        printf("Failed to open audio file.\n");

        return 0;
    }

    fseek(file, 0, SEEK_END);

    int fileLength = ftell(file) + 1;

    fseek(file, 0, SEEK_SET);

    char* buffer = new char[fileLength];

    fread(buffer, 1, fileLength, file);

    fclose(file);

    std::string dataString(buffer, fileLength);

    delete[] buffer;

    return fromWAVString(dataString);
}

Sound* Sound::fromWAVString(const std::string &data)
{
    Sound* sendback = new Sound();

    const char* buffer = data.c_str();

    // Check if it is a RIFF file
    std::string riffString(&buffer[0], 4);
    if (riffString != "RIFF")
    {
        fprintf(stderr, "File format not readable (RIFF file required, got %s)\n", riffString.c_str());
        return 0;
    }
    // Read file size
//    fseek(file, 4, SEEK_SET);
//    fread(fourBytes, 1, 4, file);
    int32_t fileSize = 0;
    std::memcpy(&fileSize, &buffer[4], 4);
    // Check if it is a WAVE file
//    fseek(file, 8, SEEK_SET);
//    fread(fourBytes, 1, 4, file);
    std::string waveString(&buffer[8], 4);
    if (waveString != "WAVE")
    {
        fprintf(stderr, "File format not readable (WAVE file required)\n");
        return 0;
    }

    // Check next part of header
//    fseek(file, 12, SEEK_SET);
//    fread(fourBytes, 1, 4, file);
    std::string formatString(&buffer[12], 4);
    if (formatString != "fmt ")
    {
        printf("Wrong Header\n");
        return 0;
    }

    // Header size
//    fseek(file, 16, SEEK_SET);
//    fread(fourBytes, 1, 4, file);
    int32_t headerSize = 0;
    memcpy(&headerSize, &buffer[16], 4);

    // Format
//    fseek(file, 20, SEEK_SET);
//    fread(twoBytes, 1, 2, file);
    int16_t format = 0;
    memcpy(&format, &buffer[20], 2);

    if (format != 0x01)
    {
        fprintf(stderr, "Format is not supported (PCM required)\n");
        return 0;
    }

    // Channels
//    fseek(file, 22, SEEK_SET);
//    fread(twoBytes, 1, 2, file);
    memcpy(&sendback->channels, &buffer[22], 2);

    // Sample rate
//    fseek(file, 24, SEEK_SET);
//    fread(fourBytes, 1, 4, file);
    memcpy(&sendback->sampleRate, &buffer[24], 4);

    // Bytes per Second
//    fseek(file, 28, SEEK_SET);
//    fread(fourBytes, 1, 4, file);
    memcpy(&sendback->bytesPerSecond, &buffer[28], 4);

    // Block align
//    fseek(file, 32, SEEK_SET);
//    fread(twoBytes, 1, 2, file);
    int16_t blockAlign = 0;
    memcpy(&blockAlign, &buffer[32], 2);

    // Bits per sample
//    fseek(file, 34, SEEK_SET);
//    fread(twoBytes, 1, 2, file);
    memcpy(&sendback->bitsPerSample, &buffer[34], 2);

    // Data
//    fseek(file, 36, SEEK_SET);
//    fread(fourBytes, 1, 4, file);
    std::string dataHeader(&buffer[36], 4);

    if (dataHeader != "data")
    {
        fprintf(stderr, "Wrong format, Data expected\n");
        return 0;
    }

    // Data length
//    fseek(file, 40, SEEK_SET);
//    fread(fourBytes, 1, 4, file);
    memcpy(&sendback->dataSize, &buffer[40], 4);

    sendback->data = new char[sendback->dataSize];
//    fseek(file, 44, SEEK_SET);
    memcpy(sendback->data, &data[44], sendback->dataSize);
//    if (fread(sendback->data, 1, sendback->dataSize, file) != sendback->dataSize)
//    {
//        fprintf(stderr, "Error reading sound data.\n");
//    }

    return sendback;
}

Sound::Sound() :
    dataSize(0),
    bitsPerSample(0),
    bytesPerSecond(0),
    sampleRate(0),
    channels(0),
    data(0)
{
}

Sound::~Sound()
{
    delete[] data;
}
