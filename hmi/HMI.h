#ifndef HMI_H
#define HMI_H

#include <thread>
#include <unordered_map>
#include <functional>
#include <linux/input-event-codes.h>

class HMI
{
public:

    static HMI* getInstance();

    ~HMI();
    void setListener(const int &key, const std::function<void(void)> listener);
    void clearListeners();

private:
    HMI();

    bool running;

    bool watchArrow;

    void callListeners(const int &key);

    void homePressed(bool pressed);
    void watchArrowPressed(int key, bool pressed);

    std::unordered_map<int, std::function<void (void)>> listeners;

    void runKeyThread(int device);
    void runSpecialThread(int device);

    std::thread keyThread;
    std::thread specialThread;
};

#endif // HMI_H
