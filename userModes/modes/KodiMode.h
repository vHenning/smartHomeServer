#ifndef KODIMODE_H
#define KODIMODE_H

#include "../UserMode.h"

class KodiMode : public UserMode
{
public:
    KodiMode();

private:
    void turnOnKodi();
    void turnOnHMI();
    void turnOnBeamer();
};

#endif // KODIMODE_H
