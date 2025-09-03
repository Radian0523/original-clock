#include "PassiveBuzzer.h"

// =======================
// PassiveBuzzer
// =======================
PassiveBuzzer::PassiveBuzzer(int pin) : buzzerPin(pin)
{
    pinMode(buzzerPin, OUTPUT);
}

void PassiveBuzzer::playTone(int freq, int dur)
{
    tone(buzzerPin, freq, dur);
}

void PassiveBuzzer::stopTone()
{
    noTone(buzzerPin);
}

void PassiveBuzzer::playMelody(Melody *melody)
{
    currentMelody = melody;
    if (currentMelody)
        currentMelody->start();
}

void PassiveBuzzer::update()
{
    if (currentMelody)
        currentMelody->update(*this);
}

// =======================
// Melody
// =======================
Melody::Melody() : noteCount(0), index(0), lastTime(0) {}

void Melody::addNote(int freq, int dur)
{
    if (noteCount < MAX_NOTES)
    {
        notes[noteCount].frequency = freq;
        notes[noteCount].duration = dur;
        noteCount++;
    }
}

void Melody::start()
{
    index = 0;
    lastTime = millis();
}

void Melody::update(PassiveBuzzer &buzzer)
{
    if (index >= noteCount)
        return;

    unsigned long now = millis();
    if (now - lastTime >= (unsigned long)notes[index].duration)
    {
        index++;
        lastTime = now;
    }

    if (index < noteCount)
    {
        buzzer.playTone(notes[index].frequency, notes[index].duration);
    }
    else
    {
        buzzer.stopTone();
    }
}
