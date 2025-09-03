#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include <Arduino.h>
#include "Channel.h"

#define MAX_CHANNELS 10

class ChannelManager
{
public:
    ChannelManager(LcdDisplay &lcdRef);
    void addChannel(Channel *c);
    void setChannel(int idx);
    void update();

private:
    Channel *channels[MAX_CHANNELS];
    LcdDisplay &lcd;
    int currentIndex;
};

#endif
