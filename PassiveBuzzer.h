#ifndef PASSIVE_BUZZER_H
#define PASSIVE_BUZZER_H

#include <Arduino.h>
#include "pitches.h"

class PassiveBuzzer
{
public:
    // コンストラクタ
    PassiveBuzzer(uint8_t pin);

    // 初期化
    void begin();

    // 単音再生
    void playTone(uint16_t frequency, unsigned long duration = 0);
    void stopTone();

    // 効果音（SE）
    void playBeep();
    void playAlert();
    void playSuccess();
    void playError();
    void playClick();

    // メロディ再生
    void playMelody(const uint16_t *melody, const uint16_t *durations, uint16_t length, uint8_t tempo = 120);
    void stopMelody();

    // BGM再生
    void playBGM(const uint16_t *melody, const uint16_t *durations, uint16_t length, uint8_t tempo = 120);
    void stopBGM();

    // 非ブロッキング更新（loopで呼び出す）
    void update();

    // 状態確認
    bool isPlaying() const;
    bool isBGMPlaying() const;

private:
    uint8_t buzzerPin;
    bool isPlayingTone;
    bool isPlayingMelody;
    bool isPlayingBGM;

    const uint16_t *currentMelody;
    const uint16_t *currentDurations;
    uint16_t melodyLength;
    uint16_t currentNote;
    unsigned long noteStartTime;
    unsigned long noteDuration;
    uint8_t bgmTempo;

    // 効果音用メロディ
    static const uint16_t BEEP_MELODY[2];
    static const uint16_t BEEP_DURATIONS[2];

    static const uint16_t ALERT_MELODY[4];
    static const uint16_t ALERT_DURATIONS[4];

    static const uint16_t SUCCESS_MELODY[8];
    static const uint16_t SUCCESS_DURATIONS[8];

    static const uint16_t ERROR_MELODY[6];
    static const uint16_t ERROR_DURATIONS[6];

    static const uint16_t CLICK_MELODY[1];
    static const uint16_t CLICK_DURATIONS[1];

    void playNextNote();
};

#endif