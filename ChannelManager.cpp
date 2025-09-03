#include "ChannelManager.h"
#include "LCDDisplay.h"

ChannelManager::ChannelManager(LcdDisplay &lcdRef) : lcd(lcdRef), currentIndex(-1)
{
    for (int i = 0; i < MAX_CHANNELS; i++)
        channels[i] = nullptr;
}

void ChannelManager::addChannel(Channel *c)
{
    for (int i = 0; i < MAX_CHANNELS; i++)
    {
        if (channels[i] == nullptr)
        {
            channels[i] = c;
            return;
        }
    }
}

void ChannelManager::setChannel(int idx)
{
    if (idx >= 0 && idx < MAX_CHANNELS && channels[idx] != nullptr)
        currentIndex = idx;
}

void ChannelManager::update()
{
    if (currentIndex >= 0 && channels[currentIndex] != nullptr)
    {
        channels[currentIndex]->show(lcd);
        channels[currentIndex]->update(lcd);
    }
}
