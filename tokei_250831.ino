// ================================
// 必要なライブラリをインクルード
// ================================
#include <Wire.h> // I2C通信用
#include "Rtc.h"
#include "LcdDisplay.h"
#include "LedMatrix.h"
#include "PassiveBuzzer.h"

// ================================
// ピン定義
// ================================
#define LCD_RS 7
#define LCD_EN 8
#define LCD_D4 9
#define LCD_D5 10
#define LCD_D6 11
#define LCD_D7 12

#define LED_DIN 6
#define LED_CS 5
#define LED_CLK 4

#define BUZZER_PIN 3

#define IR_REMOTE_RECEIVER 13
// ================================
// 定数定義
// ================================
#define DEFAULT_LED_MATRIX_INTENSITY 1

// ================================
// グローバル変数
// ================================

Rtc rtc;
LcdDisplay lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LedMatrix ledMatrix(LED_DIN, LED_CLK, LED_CS, 1);
PassiveBuzzer buzzer(BUZZER_PIN);

// ================================
// 関数群
// ================================
// 現在時刻をLCDにうつす
void displayCurrentTime()
{
    lcd.printText(0, 1, rtc.getFormattedTime());
}
// 一秒ごとにLCDに映る時刻を更新
void checkAndUpdateDisplay()
{
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate >= 1000)
    {
        lastUpdate = millis();
        displayCurrentTime();
    }
}

// ================================
// setup関数
// ================================
void setup()
{
    // initialize serial port
    Serial.begin(9600);
    while (!Serial)
        ;
    // initialize rtc
    if (!rtc.begin())
    {
        Serial.println("RTCの初期化に失敗しました");
        while (1)
            ; // 停止
    }
    Serial.println("RTCの初期化に成功しました");
    // initialize lcd display
    lcd.begin();
    // initialize led matrix
    ledMatrix.begin();
    ledMatrix.setIntensity(DEFAULT_LED_MATRIX_INTENSITY);
    // TODO : kari
    lcd.printText(0, 0, "Miyuu is No.1!");
    ledMatrix.displaySmiley(true);
    // buzzer.playSuccess();

    displayCurrentTime();
}

// ================================
// loop関数
// ================================
void loop()
{
    checkAndUpdateDisplay();
    ledMatrix.update();
    buzzer.update();

    // kari
    static unsigned long lastChange = 0;
    if (millis() - lastChange > 10000)
    {
        lastChange = millis();

        switch (random(6))
        {
        case 0:
            ledMatrix.startSineWave(50, 3);
            break;
        case 1:
            ledMatrix.startWaterRipple(70);
            break;
        case 2:
            ledMatrix.startParticleWave(60);
            break;
        case 3:
            ledMatrix.startMatrixRain(80);
            break;
        case 4:
            ledMatrix.startFireworks(100);
            break;
        case 5:
            ledMatrix.startPlasmaWave(40);
            break;
        }
    }
}
