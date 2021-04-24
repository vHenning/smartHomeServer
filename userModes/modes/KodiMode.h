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
    void turnOnStereo();

    /**
     * Executed if volume up/down is pressed on the HMI.
     * Checks if this mode has control over stereo. If yes, sets stereo accordingly
     * @param up - True if volume shall be turned up, false if down
     */
    void volumeListener(const bool& up);
};

#endif // KODIMODE_H
