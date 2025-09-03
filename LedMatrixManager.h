#ifndef LEDMATRIXMANAGER_H
#define LEDMATRIXMANAGER_H

#include "Arduino.h"
#include "Animation.h"
#include "LedControl.h"

#define NUM_AUTO_ANIMS 4

class LedMatrixManager
{
public:
    LedMatrixManager(LedControl &l);

    void setChannelAnimation(Animation *anim);
    void update();

    void addAutoAnimation(Animation *anim, int index);

private:
    LedControl &ledControl;
    Animation *channelAnim = nullptr;
    Animation *autoAnim = nullptr;
    Animation *currentAnim = nullptr;
    unsigned long lastSwitch = 0;
    Animation *autoAnimList[NUM_AUTO_ANIMS];

    void autoSwitchAnimation();
    void clearMatrix();
};

#endif