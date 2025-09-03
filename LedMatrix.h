#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <Arduino.h>
#include "LedControl.h"

class LedMatrix
{
public:
    // コンストラクタ
    LedMatrix(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices = 1);

    // 初期化
    void begin();

    // 基本制御
    void clear();
    void shutdown(bool status);
    void setIntensity(uint8_t intensity);

    // ピクセル操作
    void setPixel(uint8_t x, uint8_t y, bool state);
    void setRow(uint8_t address, uint8_t row, uint8_t value);
    void setColumn(uint8_t address, uint8_t col, uint8_t value);

    // 表示モード
    void displayChar(uint8_t address, char character);
    void displayStaticText(const String &text);
    void startScrollingText(const String &text, unsigned long scrollInterval = 150);
    void startBlinkingText(const String &text, unsigned long blinkInterval = 500, uint8_t blinkCount = 3);
    void displaySmiley(bool happy = true);
    void displayHeart();

    // 波・アニメーション系メソッド
    void startSineWave(uint8_t speed = 50, uint8_t amplitude = 3);
    void startWaterRipple(uint8_t speed = 70);
    void startParticleWave(uint8_t speed = 60);
    void startMatrixRain(uint8_t speed = 80);
    void startFireworks(uint8_t speed = 100);
    void startPlasmaWave(uint8_t speed = 40);

    // 非ブロッキング更新（loopで定期的に呼び出す）
    void update();

    // ピクセル状態取得
    bool getPixelState(uint8_t x, uint8_t y) const;

    // アニメーション制御
    void stopAnimation();
    bool isAnimating() const;

private:
    LedControl lc;
    uint8_t numDevices;
    uint8_t maxX;

    // アニメーション状態
    enum AnimationType
    {
        NONE,
        SCROLLING,
        BLINKING,
        SINE_WAVE,
        WATER_RIPPLE,
        PARTICLE_WAVE,
        MATRIX_RAIN,
        FIREWORKS,
        PLASMA_WAVE
    };

    AnimationType currentAnimation;
    String animationText;
    unsigned long animationInterval;
    unsigned long lastUpdateTime;
    uint8_t animationCounter;
    uint8_t maxBlinks;
    int scrollPosition;

    // アニメーション用変数
    float wavePhase;
    uint8_t rippleCenterX;
    uint8_t rippleCenterY;
    uint8_t particlePos;
    uint8_t matrixRainCounter;
    uint8_t fireworkCounter;
    float plasmaTime;

    // フォントと図形データ
    static const uint8_t font[96][8];
    static const uint8_t smileyHappy[8];
    static const uint8_t smileySad[8];
    static const uint8_t heart[8];

    // プライベートメソッド
    void updateScrolling();
    void updateBlinking();
    void updateSineWave();
    void updateWaterRipple();
    void updateParticleWave();
    void updateMatrixRain();
    void updateFireworks();
    void updatePlasmaWave();
};

#endif