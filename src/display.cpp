/*
  N32B Macros Firmware v4.x.x
  MIT License

  Copyright (c) 2024 SHIK
*/

#include "display.h"

#ifndef N32Bv3
const static byte chars[] = 
{
    B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000, B01111111, B01111011
};

const static byte hexes[] = 
{
    B01111110,  // 0
    B00110000,  // 1
    B01101101,  // 2
    B01111001,  // 3
    B00110011,  // 4
    B01011011,  // 5
    B01011111,  // 6
    B01110000,  // 7
    B01111111,  // 8
    B01111011,  // 9
    B01110111,  // A
    B00011111,  // b
    B01001110,  // C
    B00111101,  // d
    B01001111,  // E
    B01000111   // F
};
#endif

// Auto clear the display
void N32B_DISPLAY::clearDisplay(uint16_t readInterval)
{
    if (millis() - displayOffTimer >= readInterval)
    {
        clear();
#ifdef N32Bv3
        flush();
#endif
    }
}

void N32B_DISPLAY::showValue(uint8_t value)
{
    // TODO: Force last value to update the display to prevent missing the last value showing
    // if (millis() - lastUpdateTime < 10)
    //     return;

#ifndef N32Bv3
    clear();
    printDigit(value);
    if (value > 99)
    {
        write(1, chars[value % 10] | B10000000);
    }
    displayOffTimer = millis();
#else
    setNumber(value);
    flush();

    // lastUpdateTime = millis();
    displayOffTimer = millis();
#endif
}

// Blink the decimal points
#ifndef N32Bv3
void N32B_DISPLAY::blinkDot(uint8_t dotSide)
{
    clear();
    write(dotSide, B10000000);
    displayOffTimer = millis();
}
#else
void N32B_DISPLAY::blinkDot(uint8_t nDigit)
{
    setDecimalPoint(nDigit);
    flush();

    displayOffTimer = millis();
}
#endif

void N32B_DISPLAY::showChannelNumber(uint8_t channelNumber)
{
#ifndef N32Bv3
    clear();
    printDigit(channelNumber);
#else
    setNumber(channelNumber);
    setDigit(2, MAX7219DIGIT(B01001110));
    flush();
#endif
    displayOffTimer = millis();
}

void N32B_DISPLAY::showMIDIVal(uint8_t ccVal)
{
#ifndef N32Bv3
    clear();
    printHexFromInt(ccVal);
#else
    setNumber(channelNumber);
    setDigit(2, MAX7219DIGIT(B01001110));
    flush();
#endif
    displayOffTimer = millis();
}

void N32B_DISPLAY::showPresetNumber(uint8_t presetNumber)
{
#ifndef N32Bv3
    clear();
    printDigit(presetNumber + 1);
    write(2, B01100111);
#else
    setNumber(presetNumber + 1);
    setDigit(2, MAX7219DIGIT(B01100111));
    flush();
#endif
    displayOffTimer = millis();
}

void N32B_DISPLAY::showStartUpAnimation()
{
#ifndef N32Bv3
    uint8_t delayTime = 160;
    uint8_t repeats = 5;
    for (uint8_t i = 0; i < repeats; i++)
    {
        write(1, B00000001);
        write(2, B00001000);

        delay(delayTime);
        clear();

        write(1, B01000000);
        write(2, B00000001);
        delay(delayTime);
        clear();

        write(1, B00001000);
        write(2, B01000000);
        delay(delayTime);
        clear();
    }
#else
    uint8_t delayTime = 120;
    clear();

    setDigit(2, MAX7219DIGIT(B00001000));
    setDigit(1, MAX7219DIGIT(B00000000));
    setDigit(0, MAX7219DIGIT(B00000000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B00001000));
    setDigit(1, MAX7219DIGIT(B00001000));
    setDigit(0, MAX7219DIGIT(B00000000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B00001000));
    setDigit(1, MAX7219DIGIT(B00001000));
    setDigit(0, MAX7219DIGIT(B00001000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B00001000));
    setDigit(1, MAX7219DIGIT(B00001000));
    setDigit(0, MAX7219DIGIT(B00011000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B00001000));
    setDigit(1, MAX7219DIGIT(B00001000));
    setDigit(0, MAX7219DIGIT(B00111000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B00001000));
    setDigit(1, MAX7219DIGIT(B00001000));
    setDigit(0, MAX7219DIGIT(B01111000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B00001000));
    setDigit(1, MAX7219DIGIT(B01001000));
    setDigit(0, MAX7219DIGIT(B01111000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B01001000));
    setDigit(1, MAX7219DIGIT(B01001000));
    setDigit(0, MAX7219DIGIT(B01111000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B01001010));
    setDigit(1, MAX7219DIGIT(B01001000));
    setDigit(0, MAX7219DIGIT(B01111000));
    flush();

    delay(delayTime);

    setDigit(2, MAX7219DIGIT(B01001110));
    setDigit(1, MAX7219DIGIT(B01001000));
    setDigit(0, MAX7219DIGIT(B01111000));
    flush();

    delay(300);

    clear();
    flush();
#endif
    displayOffTimer = millis();
}

// Show animation after factory reset (infinity symbol animation)
void N32B_DISPLAY::factoryResetAnimation()
{
    const uint8_t delayTime = 100;
    const uint8_t repeats = 3;

    for (uint8_t i = 0; i < repeats; i++)
    {
#ifndef N32Bv3
        clear();

        write(2, B00010000);
        delay(delayTime);
        clear();

        write(2, B00011000);
        delay(delayTime);
        clear();

        write(2, B00011100);
        delay(delayTime);
        clear();

        write(2, B00001101);
        delay(delayTime);
        clear();

        write(2, B00000101);
        write(1, B00000001);
        delay(delayTime);
        clear();

        write(2, B00000001);
        write(1, B00100001);
        delay(delayTime);
        clear();

        write(1, B01100001);
        delay(delayTime);
        clear();

        write(1, B01100010);
        delay(delayTime);
        clear();

        write(1, B01000010);
        write(2, B00010000);
        delay(delayTime);
        clear();
#else
        clear();

        setDigit(1, MAX7219DIGIT(B00010000));
        flush();
        delay(delayTime);

        setDigit(1, MAX7219DIGIT(B00011000));
        flush();
        delay(delayTime);

        setDigit(1, MAX7219DIGIT(B00011100));
        flush();
        delay(delayTime);

        setDigit(1, MAX7219DIGIT(B00001101));
        flush();
        delay(delayTime);

        setDigit(1, MAX7219DIGIT(B00000101));
        setDigit(0, MAX7219DIGIT(B00000001));
        flush();
        delay(delayTime);

        setDigit(1, MAX7219DIGIT(B00000001));
        setDigit(0, MAX7219DIGIT(B00100001));
        flush();
        delay(delayTime);

        setDigit(0, MAX7219DIGIT(B01100001));
        flush();
        delay(delayTime);

        setDigit(0, MAX7219DIGIT(B01100010));
        flush();
        delay(delayTime);

        setDigit(0, MAX7219DIGIT(B01000010));
        setDigit(1, MAX7219DIGIT(B00010000));
        flush();
        delay(delayTime);

        clear();
        flush();
#endif
    }
}

// Show save message "Sv."
void N32B_DISPLAY::showSaveMessage()
{
#ifndef N32Bv3
    clear();
    write(2, B01011011);
    write(1, B00011100);
    delay(300);
    write(1, B10011100);
    delay(300);
    write(1, B00011100);
    delay(300);
    write(1, B10011100);
    delay(300);
#else
    const uint16_t delayTime = 300;

    clear();

    setDigit(1, MAX7219DIGIT(B01011011));
    setDigit(0, MAX7219DIGIT(B00011100));
    flush();
    delay(delayTime);

    const uint8_t repeats = 3;
    bool bDecimalPoint = true;

    for (uint8_t i = 0; i < repeats; i++)
    {
        setDecimalPoint(0, bDecimalPoint);
        flush();
        delay(delayTime);

        bDecimalPoint = !bDecimalPoint;
    }
#endif
}

void N32B_DISPLAY::printHexFromInt(uint8_t intVal)
{
    clear();

    uint8_t firstByte   = intVal >> 4;
    uint8_t secondByte  = intVal & 0x0F;

    write(1, hexes[secondByte]);
    write(2, hexes[firstByte]);
    
    displayOffTimer = millis();
}
