#include "PassiveBuzzer.h"

// 効果音メロディ定義
const uint16_t PassiveBuzzer::BEEP_MELODY[2] = {NOTE_C5, 0};
const uint16_t PassiveBuzzer::BEEP_DURATIONS[2] = {100, 50};

const uint16_t PassiveBuzzer::ALERT_MELODY[4] = {NOTE_G5, NOTE_E5, NOTE_G5, 0};
const uint16_t PassiveBuzzer::ALERT_DURATIONS[4] = {200, 200, 200, 100};

const uint16_t PassiveBuzzer::SUCCESS_MELODY[8] = {NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, NOTE_G5, NOTE_E5, NOTE_C5, 0};
const uint16_t PassiveBuzzer::SUCCESS_DURATIONS[8] = {150, 150, 150, 300, 150, 150, 150, 100};

const uint16_t PassiveBuzzer::ERROR_MELODY[6] = {NOTE_C3, NOTE_G2, NOTE_C2, NOTE_G2, NOTE_C3, 0};
const uint16_t PassiveBuzzer::ERROR_DURATIONS[6] = {200, 200, 200, 200, 200, 100};

const uint16_t PassiveBuzzer::CLICK_MELODY[1] = {NOTE_C6};
const uint16_t PassiveBuzzer::CLICK_DURATIONS[1] = {10};

// コンストラクタ
PassiveBuzzer::PassiveBuzzer(uint8_t pin)
    : buzzerPin(pin), isPlayingTone(false), isPlayingMelody(false),
      isPlayingBGM(false), currentMelody(nullptr), currentDurations(nullptr),
      melodyLength(0), currentNote(0), noteStartTime(0), noteDuration(0), bgmTempo(120) {}

void PassiveBuzzer::begin()
{
    pinMode(buzzerPin, OUTPUT);
    stopTone();
}

void PassiveBuzzer::playTone(uint16_t frequency, unsigned long duration)
{
    stopTone();
    if (frequency > 0)
    {
        tone(buzzerPin, frequency, duration);
        isPlayingTone = (duration == 0); // 持続時間0の場合は手動で停止が必要
    }
}

void PassiveBuzzer::stopTone()
{
    noTone(buzzerPin);
    isPlayingTone = false;
}

void PassiveBuzzer::playBeep()
{
    playMelody(BEEP_MELODY, BEEP_DURATIONS, 2);
}

void PassiveBuzzer::playAlert()
{
    playMelody(ALERT_MELODY, ALERT_DURATIONS, 4);
}

void PassiveBuzzer::playSuccess()
{
    playMelody(SUCCESS_MELODY, SUCCESS_DURATIONS, 8);
}

void PassiveBuzzer::playError()
{
    playMelody(ERROR_MELODY, ERROR_DURATIONS, 6);
}

void PassiveBuzzer::playClick()
{
    playMelody(CLICK_MELODY, CLICK_DURATIONS, 1);
}

void PassiveBuzzer::playMelody(const uint16_t *melody, const uint16_t *durations, uint16_t length, uint8_t tempo)
{
    stopMelody();
    currentMelody = melody;
    currentDurations = durations;
    melodyLength = length;
    currentNote = 0;
    bgmTempo = tempo;
    isPlayingMelody = true;
    noteStartTime = millis();
    playNextNote();
}

void PassiveBuzzer::playBGM(const uint16_t *melody, const uint16_t *durations, uint16_t length, uint8_t tempo)
{
    stopBGM();
    currentMelody = melody;
    currentDurations = durations;
    melodyLength = length;
    currentNote = 0;
    bgmTempo = tempo;
    isPlayingBGM = true;
    noteStartTime = millis();
    playNextNote();
}

void PassiveBuzzer::stopMelody()
{
    if (isPlayingMelody)
    {
        stopTone();
        isPlayingMelody = false;
    }
}

void PassiveBuzzer::stopBGM()
{
    if (isPlayingBGM)
    {
        stopTone();
        isPlayingBGM = false;
    }
}

void PassiveBuzzer::update()
{
    if (isPlayingMelody || isPlayingBGM)
    {
        unsigned long currentTime = millis();

        if (currentTime - noteStartTime >= noteDuration)
        {
            currentNote++;
            if (currentNote >= melodyLength)
            {
                if (isPlayingBGM)
                {
                    // BGMの場合はループ
                    currentNote = 0;
                }
                else
                {
                    // メロディの場合は停止
                    stopMelody();
                    return;
                }
            }
            noteStartTime = currentTime;
            playNextNote();
        }
    }
}

void PassiveBuzzer::playNextNote()
{
    if (currentNote < melodyLength)
    {
        uint16_t note = currentMelody[currentNote];
        uint16_t duration = currentDurations[currentNote];

        // テンポ調整
        noteDuration = (60000 / bgmTempo) * duration / 100;

        if (note == 0)
        {
            stopTone();
        }
        else
        {
            playTone(note, noteDuration);
        }
    }
}

bool PassiveBuzzer::isPlaying() const
{
    return isPlayingTone || isPlayingMelody;
}

bool PassiveBuzzer::isBGMPlaying() const
{
    return isPlayingBGM;
}