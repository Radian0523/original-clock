#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino.h>
#include <LedControl.h>

class Animation
{
public:
    virtual void start() = 0;
    virtual void update() = 0;
    virtual bool isFinished() { return false; }
    virtual ~Animation() {}
};

#endif