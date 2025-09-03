#include "LedMatrix.h"

// コンストラクタでアニメーション変数を初期化
LedMatrix::LedMatrix(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices)
    : lc(dataPin, clkPin, csPin, numDevices), numDevices(numDevices),
      currentAnimation(NONE), lastUpdateTime(0), animationCounter(0),
      scrollPosition(0), maxBlinks(0), wavePhase(0), rippleCenterX(3),
      rippleCenterY(3), particlePos(0), matrixRainCounter(0),
      fireworkCounter(0), plasmaTime(0)
{
    maxX = numDevices * 8 - 1;
}

void LedMatrix::begin()
{
    for (int address = 0; address < numDevices; address++)
    {
        lc.shutdown(address, false);
        lc.setIntensity(address, 8);
        lc.clearDisplay(address);
    }
    currentAnimation = NONE;
}

void LedMatrix::clear()
{
    for (int address = 0; address < numDevices; address++)
    {
        lc.clearDisplay(address);
    }
}

void LedMatrix::shutdown(bool status)
{
    for (int address = 0; address < numDevices; address++)
    {
        lc.shutdown(address, status);
    }
}

void LedMatrix::setIntensity(uint8_t intensity)
{
    for (int address = 0; address < numDevices; address++)
    {
        lc.setIntensity(address, intensity);
    }
}

void LedMatrix::setPixel(uint8_t x, uint8_t y, bool state)
{
    if (x > maxX || y > 7)
        return;

    uint8_t address = x / 8;
    uint8_t col = x % 8;
    lc.setLed(address, col, y, state);
}

void LedMatrix::setRow(uint8_t address, uint8_t row, uint8_t value)
{
    if (address < numDevices && row < 8)
    {
        lc.setRow(address, row, value);
    }
}

void LedMatrix::setColumn(uint8_t address, uint8_t col, uint8_t value)
{
    if (address < numDevices && col < 8)
    {
        lc.setColumn(address, col, value);
    }
}

// 簡易的なフォントデータ（数字とアルファベットのみ）
const uint8_t LedMatrix::font[96][8] = {
    // スペース (0x20)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // ! (0x21)
    {0x00, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00},
    // 数字 0-9 (0x30-0x39)
    {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00}, // 0
    {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00}, // 1
    {0x00, 0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x00}, // 2
    {0x00, 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00, 0x00}, // 3
    {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00}, // 4
    {0x00, 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00}, // 5
    {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00}, // 6
    {0x00, 0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00}, // 7
    {0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00}, // 8
    {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00}, // 9
    // アルファベット A-Z など必要な文字を追加...
    // 簡易的に数字のみでテストする場合は、とりあえず上記まででOK
};

void LedMatrix::displayChar(uint8_t address, char character)
{
    if (address >= numDevices)
        return;

    uint8_t charIndex = character - 32;
    if (charIndex < 96)
    {
        for (uint8_t row = 0; row < 8; row++)
        {
            lc.setRow(address, row, font[charIndex][row]);
        }
    }
}

void LedMatrix::displayStaticText(const String &text)
{
    stopAnimation();
    clear();
    // 静的テキスト表示の実装（最初の1文字のみ表示など）
    if (text.length() > 0)
    {
        displayChar(0, text[0]);
    }
}

void LedMatrix::startScrollingText(const String &text, unsigned long scrollInterval)
{
    currentAnimation = SCROLLING;
    animationText = text;
    animationInterval = scrollInterval;
    scrollPosition = 0;
    lastUpdateTime = millis();
}

void LedMatrix::startBlinkingText(const String &text, unsigned long blinkInterval, uint8_t blinkCount)
{
    currentAnimation = BLINKING;
    animationText = text;
    animationInterval = blinkInterval;
    animationCounter = 0;
    maxBlinks = blinkCount * 2; // ON/OFFで2倍
    lastUpdateTime = millis();

    // 最初は表示状態で開始
    displayStaticText(text);
}

void LedMatrix::displaySmiley(bool happy)
{
    stopAnimation();
    const uint8_t *data = happy ? smileyHappy : smileySad;
    for (uint8_t row = 0; row < 8; row++)
    {
        lc.setRow(0, row, data[row]);
    }
}

void LedMatrix::displayHeart()
{
    stopAnimation();
    for (uint8_t row = 0; row < 8; row++)
    {
        lc.setRow(0, row, heart[row]);
    }
}

// アニメーション開始メソッド
void LedMatrix::startSineWave(uint8_t speed, uint8_t amplitude)
{
    stopAnimation();
    currentAnimation = SINE_WAVE;
    animationInterval = speed;
    wavePhase = 0;
    lastUpdateTime = millis();
}

void LedMatrix::startWaterRipple(uint8_t speed)
{
    stopAnimation();
    currentAnimation = WATER_RIPPLE;
    animationInterval = speed;
    rippleCenterX = random(2, 6);
    rippleCenterY = random(2, 6);
    animationCounter = 0;
    lastUpdateTime = millis();
}

void LedMatrix::startParticleWave(uint8_t speed)
{
    stopAnimation();
    currentAnimation = PARTICLE_WAVE;
    animationInterval = speed;
    particlePos = 0;
    lastUpdateTime = millis();
}

void LedMatrix::startMatrixRain(uint8_t speed)
{
    stopAnimation();
    currentAnimation = MATRIX_RAIN;
    animationInterval = speed;
    matrixRainCounter = 0;
    lastUpdateTime = millis();
}

void LedMatrix::startFireworks(uint8_t speed)
{
    stopAnimation();
    currentAnimation = FIREWORKS;
    animationInterval = speed;
    fireworkCounter = 0;
    lastUpdateTime = millis();
}

void LedMatrix::startPlasmaWave(uint8_t speed)
{
    stopAnimation();
    currentAnimation = PLASMA_WAVE;
    animationInterval = speed;
    plasmaTime = 0;
    lastUpdateTime = millis();
}

// update() メソッドにアニメーションケースを追加
void LedMatrix::update()
{
    unsigned long currentTime = millis();

    if (currentTime - lastUpdateTime >= animationInterval)
    {
        lastUpdateTime = currentTime;

        switch (currentAnimation)
        {
        case SCROLLING:
            updateScrolling();
            break;
        case BLINKING:
            updateBlinking();
            break;
        case SINE_WAVE:
            updateSineWave();
            break;
        case WATER_RIPPLE:
            updateWaterRipple();
            break;
        case PARTICLE_WAVE:
            updateParticleWave();
            break;
        case MATRIX_RAIN:
            updateMatrixRain();
            break;
        case FIREWORKS:
            updateFireworks();
            break;
        case PLASMA_WAVE:
            updatePlasmaWave();
            break;
        case NONE:
        default:
            break;
        }
    }
}

// アニメーション実装
void LedMatrix::updateSineWave()
{
    clear();
    wavePhase += 0.2;

    for (uint8_t x = 0; x <= maxX; x++)
    {
        // サイン波の計算
        float y = 3.5 + 3.0 * sin(wavePhase + x * 0.5);
        uint8_t ledY = (uint8_t)y;

        if (ledY < 8)
        {
            setPixel(x, ledY, true);
            // 近くのLEDも点灯して滑らかに
            if (ledY > 0)
                setPixel(x, ledY - 1, true);
            if (ledY < 7)
                setPixel(x, ledY + 1, true);
        }
    }
}

void LedMatrix::updateWaterRipple()
{
    clear();
    animationCounter++;

    for (uint8_t x = 0; x < 8; x++)
    {
        for (uint8_t y = 0; y < 8; y++)
        {
            // 中心からの距離
            float dx = x - rippleCenterX;
            float dy = y - rippleCenterY;
            float distance = sqrt(dx * dx + dy * dy);

            // 波の計算
            float wave = sin(distance - animationCounter * 0.3);
            if (wave > 0.7)
            {
                setPixel(x, y, true);
            }
        }
    }

    if (animationCounter > 20)
    {
        // 新しい波を開始
        rippleCenterX = random(2, 6);
        rippleCenterY = random(2, 6);
        animationCounter = 0;
    }
}

void LedMatrix::updateParticleWave()
{
    clear();
    particlePos = (particlePos + 1) % 16;

    for (uint8_t x = 0; x <= maxX; x++)
    {
        // 粒子の波を描画
        uint8_t height = 3 + abs(7 - abs(x - particlePos));
        for (uint8_t y = 0; y < height && y < 8; y++)
        {
            setPixel(x, y, true);
        }
    }
}

void LedMatrix::updateMatrixRain()
{
    // すべての列を1ドット下にシフト
    for (uint8_t x = 0; x <= maxX; x++)
    {
        for (uint8_t y = 7; y > 0; y--)
        {
            bool state = getPixelState(x, y - 1);
            setPixel(x, y, state);
        }
        // 最上段をランダムに点灯
        if (random(10) < 3)
        {
            setPixel(x, 0, true);
        }
        else
        {
            setPixel(x, 0, false);
        }
    }
}

void LedMatrix::updateFireworks()
{
    clear();
    fireworkCounter++;

    if (fireworkCounter < 10)
    {
        // 打ち上げ
        uint8_t height = fireworkCounter;
        setPixel(3, 7 - height, true);
        setPixel(4, 7 - height, true);
    }
    else if (fireworkCounter < 25)
    {
        // 爆発
        uint8_t explosionSize = fireworkCounter - 10;
        for (uint8_t i = 0; i < 8; i++)
        {
            float angle = i * PI / 4;
            uint8_t x = 4 + explosionSize * cos(angle);
            uint8_t y = 4 + explosionSize * sin(angle);
            if (x < 8 && y < 8)
            {
                setPixel(x, y, true);
            }
        }
    }
    else
    {
        // リセット
        fireworkCounter = 0;
    }
}

void LedMatrix::updatePlasmaWave()
{
    clear();
    plasmaTime += 0.1;

    for (uint8_t x = 0; x < 8; x++)
    {
        for (uint8_t y = 0; y < 8; y++)
        {
            // プラズマ効果の計算
            float value = 0.5 + 0.5 * sin(
                                          x * 0.3 +
                                          y * 0.4 +
                                          plasmaTime +
                                          sin(plasmaTime * 0.7 + x * 0.2) +
                                          cos(plasmaTime * 0.5 + y * 0.3));

            if (value > 0.7)
            {
                setPixel(x, y, true);
            }
        }
    }
}

// ピクセル状態取得メソッド（必要に応じて追加）
bool LedMatrix::getPixelState(uint8_t x, uint8_t y) const
{
    // 簡易実装（実際の状態管理が必要な場合は修正）
    // ここでは常にfalseを返すダミー実装
    return false;
}

void LedMatrix::updateScrolling()
{
    clear();

    for (int j = 0; j < numDevices; j++)
    {
        for (int k = 0; k < 8; k++)
        {
            int charPos = (scrollPosition + k) / 8;
            if (charPos < animationText.length())
            {
                uint8_t charIndex = animationText[charPos] - 32;
                if (charIndex < 96)
                {
                    uint8_t columnData = font[charIndex][(scrollPosition + k) % 8];
                    lc.setColumn(j, k, columnData);
                }
            }
        }
    }

    scrollPosition++;
    if (scrollPosition > animationText.length() * 8 + numDevices * 8)
    {
        scrollPosition = 0;
    }
}

void LedMatrix::updateBlinking()
{
    animationCounter++;

    if (animationCounter >= maxBlinks)
    {
        stopAnimation();
        return;
    }

    if (animationCounter % 2 == 0)
    {
        // 表示
        displayStaticText(animationText);
    }
    else
    {
        // 非表示
        clear();
    }
}

void LedMatrix::stopAnimation()
{
    currentAnimation = NONE;
    animationCounter = 0;
}

bool LedMatrix::isAnimating() const
{
    return currentAnimation != NONE;
}

// 図形データ
const uint8_t LedMatrix::smileyHappy[8] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100};

const uint8_t LedMatrix::smileySad[8] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10011001,
    0b10100101,
    0b01000010,
    0b00111100};

const uint8_t LedMatrix::heart[8] = {
    0b00000000,
    0b01100110,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000,
    0b00000000};