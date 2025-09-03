#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal.h> // もしくは使用するLCDライブラリ

class LcdDisplay
{
public:
    LcdDisplay(int rs, int en, int d4, int d5, int d6, int d7);

    void begin(); // LCD初期化
    void clear();
    void printText(int col, int row, const String &text);
    void setCursor(int col, int row);

private:
    LiquidCrystal lcd;
};

#endif
