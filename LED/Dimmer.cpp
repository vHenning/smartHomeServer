#include "Dimmer.h"

#include <math.h>
#include <unistd.h>

Dimmer::Dimmer(double normalBrightness, double brightnessChange)
    : brightnessChange(brightnessChange)
    , maxChanges(5)
    , dimming(false)
    , powerState(false)
{
    std::lock_guard<std::mutex> lock(changeMutex);
    last = {false, std::chrono::system_clock::now(), normalBrightness, true};

    notifierThread = std::thread(&Dimmer::notifierRunner, this);
}

void Dimmer::setDimListener(std::function<void(double)> newListener)
{
    dimListener = newListener;
}

void Dimmer::setOnOffListener(std::function<void(bool)> newListener)
{
    onOffListener = newListener;
}

void Dimmer::setPowerState(bool on)
{
    if (powerState != on)
    {
        powerState = on;
        onOffListener(on);
    }
}

void Dimmer::switchChanged(bool on)
{
    Change end;
    {
        std::lock_guard<std::mutex> lock(changeMutex);
        end = last;
    }

    double seconds = (std::chrono::system_clock::now() - end.time).count() / 1000000000.0;
    std::pair<double, bool> newBrightness;
    
    // If from on to off
    if (end.switchedOn && !on)
    {
        // Was this a short click? (then just turn on/off)
        if (seconds < DIM_THRESHOLD)
        {
            setPowerState(!powerState);
            newBrightness = std::make_pair(end.brightness, end.direction);
            printf("Power %s\n", powerState ? "on" : "off");
        }
        // Or a dim
        else
        {
            newBrightness = calculateBrightness(end.brightness, end.direction, end.time);
            printf("End dim with brightness %f\n", newBrightness.first);
        }
    }
    // if from off to on
    else if (!end.switchedOn && on)
    {
        newBrightness = std::make_pair(end.brightness, end.direction);
    }    

    std::lock_guard<std::mutex> lock(changeMutex);
    last.brightness = newBrightness.first;
    last.direction = newBrightness.second;
    last.switchedOn = on;
    last.time = std::chrono::system_clock::now();
}

double Dimmer::getBrightness()
{
    Change end;
    {
        std::lock_guard<std::mutex> lock(changeMutex);
        end = last;
    }
    if (end.switchedOn == false)
    {
        return end.brightness;
    }
    else
    {
        return calculateBrightness(end.brightness, end.direction, end.time).first;
    }
}

std::pair<double, bool> Dimmer::calculateBrightness(double startValue, bool direction, std::chrono::time_point<std::chrono::system_clock> startTime) const
{
    bool localDirection = direction;
    double change = brightnessChange * (localDirection ? 1 : -1);

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration deltaT = now - startTime;

    double seconds = deltaT.count() / 1000000000.0;

    if (seconds < DIM_THRESHOLD)
    {
        return std::make_pair(startValue, direction);
    }

    seconds = seconds - DIM_THRESHOLD;
    double cycleTime = 1.0 / brightnessChange;
    double cyclesInDiff = seconds / cycleTime;

    cyclesInDiff = cyclesInDiff - 2 * std::floor(cyclesInDiff / 2);
    
    while (seconds >= 2 * cycleTime)
    {
        seconds -= 2 * cycleTime;
    }

    // Find out if we have to change direction
    double timeToTurnaround = (localDirection ? 1.0 - startValue : startValue) * cycleTime;

    // Direction is still valid
    if (seconds < timeToTurnaround)
    {
        double newBrightness = startValue + change * seconds;
        return std::make_pair(newBrightness, localDirection);
    }
    else if (seconds < timeToTurnaround + cycleTime)
    {
        startValue = direction ? 1.0 : 0.0;
        change = -change;
        double timeSinceTurnaround = seconds - timeToTurnaround;
        double newBrightness = startValue + change * timeSinceTurnaround;
        return std::make_pair(newBrightness, !localDirection);
    }
    else
    {
        double timeSinceSecondTurnaround = seconds - (timeToTurnaround + cycleTime);
        startValue = direction ? 0.0 : 1.0;
        double newBrightness = startValue + change * timeSinceSecondTurnaround;
        return std::make_pair(newBrightness, localDirection);
    }
}

void Dimmer::notifierRunner()
{
    // Notify with a frequency of 25 Hz
    double frequency = 25; // Hz
    double cycleTime = 1.0 / frequency; // s
    double cycleTimeMicros = cycleTime * 1000000.0; // us
    
    Dimmer* instance = this;

    while (true)
    {
        usleep(cycleTimeMicros);
        Change end;
        {
            std::lock_guard<std::mutex> lock(instance->changeMutex);
            end = instance->last;
        }

        double secondsSinceLast = (std::chrono::system_clock::now() - end.time).count() / 1000000000.0;
        
        // Turn on if we were turned off but started dimming
        if (end.switchedOn && secondsSinceLast >= DIM_THRESHOLD)
        {
            setPowerState(true);
        }

        if (end.switchedOn || secondsSinceLast <= cycleTime)
        {
            instance->dimListener(instance->getBrightness());
        }
    }
}
