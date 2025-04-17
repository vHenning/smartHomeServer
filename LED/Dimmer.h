#ifndef DIMMER_H
#define DIMMER_H

#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>

class Dimmer
{
public:
    Dimmer(double normalBrightness, double brightnessChange = 0.25);

    void switchChanged(bool on);

    double getBrightness();

    void setDimListener(std::function<void(double)> newListener);

    void setOnOffListener(std::function<void(bool)> newListener);

    bool isOn();

protected:
    std::pair<double, bool> calculateBrightness(double startValue, bool direction, std::chrono::time_point<std::chrono::system_clock> startTime) const;

    void setPowerState(bool on);

    struct Change
    {
        bool switchedOn;
        std::chrono::time_point<std::chrono::system_clock> time;
        double brightness;
        bool direction;
    };

    Change last;

    std::mutex changeMutex;

    std::function<void(double)> dimListener;
    std::function<void(bool)> onOffListener;
        
    /// Brightness change per second
    double brightnessChange;

    size_t maxChanges;

    std::thread notifierThread;

    bool dimming;

    void notifierRunner();

    bool powerState;

    const double DIM_THRESHOLD = 0.5;
};

#endif // DIMMER_H