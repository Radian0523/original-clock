// ================================
// 必要なライブラリをインクルード
// ================================
#include <Wire.h> // I2C通信用
// #include <RTClib.h> // RTC用（Adafruit RTClibライブラリ）
// #include <LiquidCrystal.h> // LCD用
// #include <IRremote.h>      // IR Remote
// #include <LedControl.h> // led matrix
// #include <pitches.h>    // passive buzzer

#include "Rtc.h"
#include "LcdDisplay.h"

// ================================
// 定数定義
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
// グローバル変数
// ================================

Rtc rtc;
LcdDisplay lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
// ChannelManager channelManager(lcd);
// LEDMatrix ledMatrix; // 仮: 16x16バッファ管理クラス
// LEDMatrixManager ledManager(ledMatrix);
// RTC_DS3231 rtc; // time
// PassiveBuzzer buzzer(BUZZER_PIN);
// IRRemoteController ir(IR_PIN, channelManager);

// ================================
// 関数群
// ================================
void displayCurrentTime()
{
    lcd.printText(0, 0, rtc.getFormattedTime());
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
    // 現在時刻の表示
    displayCurrentTime();
}

// ================================
// loop関数
// ================================
void loop()
{
}
