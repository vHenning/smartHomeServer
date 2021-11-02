#ifndef KODICONTROLLER_H
#define KODICONTROLLER_H

/**
 * Controls a Kodi system running on this system
 * If active, navigation keys are being forwarded to Kodi
 */
class KodiController
{
public:
    static KodiController* getInstance();

    /**
     * Check if Kodi is active right now
     * @return
     */
    bool isActive() const;

    /**
     * Start Kodi
     */
    void start();

    /**
     * Quit Kodi
     */
    void quit();

    /**
     * Send a key to Kodi (non relevant keys are not sent)
     * @param key - Key code from HMI
     */
    void sendKey(const int& key);

private:
    KodiController();

    bool running;
};

#endif // KODICONTROLLER_H
