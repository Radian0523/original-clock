#include "Rtc.h"
#include <Wire.h>

Rtc::Rtc() : isInitialized(false) {}

bool Rtc::begin()
{
    if (!rtc.begin())
    {
        Serial.println("RTCが見つかりません");
        return false;
    }

    if (rtc.lostPower())
    {
        Serial.println("RTCの電源が落ちています、時間を設定します");
        // コンパイル時間で設定
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    isInitialized = true;
    return true;
}

unsigned long Rtc::getUnixTime()
{
    if (!isInitialized)
        return 0;
    DateTime now = rtc.now();
    return now.unixtime();
}

DateTime Rtc::getDateTime()
{
    if (!isInitialized)
    {
        // エラー時にデフォルトのDateTimeオブジェクトを返す
        return DateTime(); // デフォルトコンストラクタを使用
    }
    return rtc.now();
}

String Rtc::getFormattedTime()
{
    if (!isInitialized)
        return "N/A";
    DateTime now = rtc.now();
    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
             now.hour(), now.minute(), now.second());
    return String(buffer);
}

String Rtc::getFormattedDate()
{
    if (!isInitialized)
        return "N/A";
    DateTime now = rtc.now();
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d",
             now.year(), now.month(), now.day());
    return String(buffer);
}

bool Rtc::adjustTime(unsigned long unixTime)
{
    if (!isInitialized)
        return false;
    rtc.adjust(DateTime(unixTime));
    return true;
}

bool Rtc::adjustTime(const DateTime &dt)
{
    if (!isInitialized)
        return false;
    rtc.adjust(dt);
    return true;
}