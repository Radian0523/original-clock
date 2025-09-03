#ifndef CHANNEL_H
#define CHANNEL_H

#include <Arduino.h>
#include "LCDDisplay.h"

class Channel
{
public:
    virtual void show(LcdDisplay &lcd) = 0;
    virtual void update(LcdDisplay &lcd) = 0;
    virtual ~Channel() {}
};

#endif
