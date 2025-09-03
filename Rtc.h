#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include "RTClib.h"

class Rtc
{
public:
    Rtc();
    bool begin();                // bool型で成功可否を返す
    unsigned long getUnixTime(); // より明確な名前に
    DateTime getDateTime();      // DateTimeオブジェクトも取得可能に
    String getFormattedTime();   // フォーマットされた時間文字列
    String getFormattedDate();   // フォーマットされた日付文字列

    // 時間設定メソッド
    bool adjustTime(unsigned long unixTime);
    bool adjustTime(const DateTime &dt);

private:
    RTC_DS3231 rtc;     // 実際のRTCオブジェクト
    bool isInitialized; // 初期化状態の追跡
};

#endif