#ifndef READING_MODE_H
#define READING_MODE_H

#include "../UserMode.h"

class ReadingMode : public UserMode
{
public:
    ReadingMode();

private:

    void turnOnReadingLight();
};

#endif // READING_MODE_H
