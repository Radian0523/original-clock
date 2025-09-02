// www.elegoo.com
// 2016.12.9

/*
  LiquidCrystal Library - Hello World (改造版)

  16x2 LCDディスプレイに「固定メッセージ（I Love Miyuu）」と
  現在時刻（時:分:秒）を表示するサンプル。

  DS3231 RTC（リアルタイムクロック）を使用。
  RTCモジュールをArduinoに接続して、現在の時刻を取得する。

  配線例（LCD部分は前と同じ）:
   * DS3231 SDA → Arduino A4
   * DS3231 SCL → Arduino A5
   * DS3231 VCC → +5V
   * DS3231 GND → GND
*/

// ================================
// 必要なライブラリをインクルード
// ================================
#include <Wire.h>          // I2C通信用
#include <RTClib.h>        // RTC用（Adafruit RTClibライブラリ）
#include <LiquidCrystal.h> // LCD用
#include <IRremote.h>      // IR Remote 用
#include <LedControl.h>    // led matrix
#include <pitches.h>       // passive buzzer

// ================================
// 定数定義
// ================================
#define LED_DIN 6
#define LED_CS 5
#define LED_CLK 4
#define IR_REMOTE_RECEIVER 13
#define DIAGONAL_WAVE_INTERVAL 100

enum
{
  ON,
  OFF,
  WAIT,
} DIAGONAL_WAVE_STATE;

// ================================
// グローバル変数
// ================================
float delta_time = 0;
unsigned long prev_time = 0;
unsigned long delaytime2 = 50;    // 短めの遅延
uint32_t last_decodedRawData = 0; // 赤外線信号の前回データ保存

LedControl lc = LedControl(LED_DIN, LED_CLK, LED_CS, 1);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
RTC_DS3231 rtc;
IRrecv irrecv(IR_REMOTE_RECEIVER);

float date_timer = 0;
float diagonal_wave_timer = 0;

// ================================
// 関数群
// ================================
void translateIR()
{
  if (irrecv.decodedIRData.flags)
  {
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT!");
  }
  else
  {
    Serial.print("IR code:0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
  }

  switch (irrecv.decodedIRData.decodedRawData)
  {
  case 0xBA45FF00:
    Serial.println("POWER");
    break;
  case 0xB847FF00:
    Serial.println("FUNC/STOP");
    break;
  case 0xB946FF00:
    Serial.println("VOL+");
    break;
  case 0xBB44FF00:
    Serial.println("FAST BACK");
    break;
  case 0xBF40FF00:
    Serial.println("PAUSE");
    break;
  case 0xBC43FF00:
    Serial.println("FAST FORWARD");
    break;
  case 0xF807FF00:
    Serial.println("DOWN");
    break;
  case 0xEA15FF00:
    Serial.println("VOL-");
    break;
  case 0xF609FF00:
    Serial.println("UP");
    break;
  case 0xE619FF00:
    Serial.println("EQ");
    break;
  case 0xF20DFF00:
    Serial.println("ST/REPT");
    break;
  case 0xE916FF00:
    Serial.println("0");
    break;
  case 0xF30CFF00:
    Serial.println("1");
    break;
  case 0xE718FF00:
    Serial.println("2");
    break;
  case 0xA15EFF00:
    Serial.println("3");
    break;
  case 0xF708FF00:
    Serial.println("4");
    break;
  case 0xE31CFF00:
    Serial.println("5");
    break;
  case 0xA55AFF00:
    Serial.println("6");
    break;
  case 0xBD42FF00:
    Serial.println("7");
    break;
  case 0xAD52FF00:
    Serial.println("8");
    break;
  case 0xB54AFF00:
    Serial.println("9");
    break;
  default:
    Serial.println(" other button   ");
  }

  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(500);
}

void start_lcd()
{
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
}

void diagonal_wave()
{
  static int sum = 0;             // 今どの対角線か
  static bool lighting = true;    // 点灯フェーズか消灯フェーズか
  static unsigned long timer = 0; // 前回更新時刻

  unsigned long now = millis();
  if (now - timer < DIAGONAL_WAVE_INTERVAL)
    return; // 一定間隔待ち
  timer = now;

  // 現在のsumに対応するLEDを処理
  for (int row = 0; row < 8; row++)
  {
    int col = sum - row;
    if (col >= 0 && col < 8)
    {
      lc.setLed(0, row, col, lighting);
    }
  }

  // 次のステップへ
  sum++;
  if (sum > 14)
  {
    sum = 0;
    lighting = !lighting; // 点灯→消灯を切り替え
  }
}

void update_delta_time()
{
  unsigned long current_time = millis();
  delta_time = (current_time - prev_time);
  prev_time = current_time;
}

void update_lcd_date()
{
  date_timer += delta_time;
  if (date_timer <= 1000)
    return;
  date_timer = 0;

  DateTime now = rtc.now();
  lcd.setCursor(0, 1);

  char buffer[17];
  sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  lcd.print(buffer);
}

// ================================
// setup関数
// ================================
void setup()
{
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");

  start_lcd();
  irrecv.enableIRIn();
  lcd.begin(16, 2);
  lcd.print("I Love Miyuu");

  if (!rtc.begin())
  {
    lcd.setCursor(0, 1);
    lcd.print("RTC ERROR!");
    while (1)
      ;
  }

  if (rtc.lostPower())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  prev_time = millis();
}

// ================================
// loop関数
// ================================
void loop()
{
  update_delta_time();

  if (irrecv.decode())
  {
    translateIR();
    irrecv.resume();
  }

  update_lcd_date();
  diagonal_wave();
}
