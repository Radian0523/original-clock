#ifndef PASSIVEBUZZER_H
#define PASSIVEBUZZER_H

#include <Arduino.h>

#define MAX_NOTES 32

// 音符
struct Note
{
    int frequency;
    int duration;
};

// メロディクラス
class PassiveBuzzer; // 前方宣言

class Melody
{
public:
    Melody(); // 空メロディ
    void addNote(int freq, int dur);
    void start();
    void update(PassiveBuzzer &buzzer);

private:
    Note notes[MAX_NOTES];
    int noteCount;
    int index;
    unsigned long lastTime;
};

// パッシブブザークラス
class PassiveBuzzer
{
public:
    PassiveBuzzer(int pin);
    void playTone(int frequency, int duration);
    void stopTone();
    void playMelody(Melody *melody);
    void update();

private:
    int buzzerPin;
    Melody *currentMelody = nullptr;
};

#endif
