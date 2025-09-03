#include "LcdDisplay.h"

LcdDisplay::LcdDisplay(int rs, int en, int d4, int d5, int d6, int d7)
    : lcd(rs, en, d4, d5, d6, d7) {}

void LcdDisplay::begin()
{
    lcd.begin(16, 2); // lcd の桁数(16×2)を設定
}

void LcdDisplay::clear()
{
    lcd.clear();
}

void LcdDisplay::printText(int col, int row, const String &text)
{
    lcd.setCursor(col, row);
    lcd.print(text);
}

void LcdDisplay::setCursor(int col, int row)
{
    lcd.setCursor(col, row);
}
