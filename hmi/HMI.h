#ifndef HMI_H
#define HMI_H

#include <vector>
#include <thread>
#include <unordered_map>
#include <functional>
#include <linux/input-event-codes.h>
#include <vector>

class HMI
{
public:

    static HMI* getInstance();

    ~HMI();

    /**
     * Add a listener that is always called when the given key is pressed
     * @param key - Keypress that shall trigger calling the listener
     * @param listener - The listener to add
     */
    void setListener(const int &key, const std::function<void(void)> listener);

    /**
     * Add a listener that is always called when a key is pressed. The pressed key is passed to the listener
     * @param listener - Listener to add
     */
    void setBroadcastListener(const std::function<void (int)>& listener);

    /**
     * Clear all (normal and broadcast) listeners
     */
    void clearListeners();

private:
    HMI();

    bool running;

    bool watchArrow;

    void callListeners(const int &key);

    void homePressed(bool pressed);
    void watchArrowPressed(int key, bool pressed);

    std::unordered_map<int, std::function<void (void)>> listeners;
    std::vector<std::function<void (int)> > broadcastListeners;

    void runThread(int device);

    struct Stream
    {
        Stream() : eventNumber("") {}
        std::string eventNumber;
        std::thread thread;
    };

    std::vector<Stream*> streams;
};

#endif // HMI_H
