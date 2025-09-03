#include "LedMatrixManager.h"

LedMatrixManager::LedMatrixManager(LedControl &l) : ledControl(l), currentAnim(nullptr)
{
    for (int i = 0; i < NUM_AUTO_ANIMS; i++)
        autoAnimList[i] = nullptr;
}

void LedMatrixManager::addAutoAnimation(Animation *anim, int index)
{
    if (index >= 0 && index < NUM_AUTO_ANIMS)
        autoAnimList[index] = anim;
}

void LedMatrixManager::update()
{
    int hour = 0; // kari TODO RTC kara syutoku
    if (hour >= 23 || hour < 6)
    {
        clearMatrix();
        return;
    }

    unsigned long now = millis();
    if (now - lastSwitch >= 3600000)
    {
        lastSwitch = now;
        autoSwitchAnimation();
    }

    Animation *target = (channelAnim != nullptr) ? channelAnim : autoAnim;

    if (currentAnim != target && target != nullptr)
    {
        currentAnim = target;
        currentAnim->start();
    }

    if (currentAnim)
    {
        currentAnim->update();
        if (currentAnim->isFinished())
            autoSwitchAnimation();
    }
}

void LedMatrixManager::autoSwitchAnimation()
{
    static int idx = 0;
    idx = (idx + 1) % NUM_AUTO_ANIMS;
    autoAnim = autoAnimList[idx];
}

void LedMatrixManager::clearMatrix()
{
    for (int addr = 0; addr < ledControl.getDeviceCount(); addr++)
    {
        for (int row = 0; row < 8; row++)
        {
            ledControl.setRow(addr, row, 0);
        }
    }
}