/*
  N32B Macros Firmware v4.x.x
  MIT License

  Copyright (c) 2024 SHIK
*/

#ifndef N32B_DISPLAY_h
#define N32B_DISPLAY_h

#include <DigitLedDisplay.h>
#include <Arduino.h>
#include <vector>
#include <bitset>

class N32B_DISPLAY : public DigitLedDisplay
{
public:
  N32B_DISPLAY(uint8_t DIN, uint8_t CS, uint8_t CLK) : DigitLedDisplay(DIN, CS, CLK){};

  void clearDisplay(uint16_t readInterval = 1000);
  void showValue(uint8_t);
  void blinkDot(uint8_t);
  void showChannelNumber(uint8_t);
  void showPresetNumber(uint8_t);
  void showStartUpAnimation();
  void factoryResetAnimation();
  void showSaveMessage();

private:
  unsigned long displayOffTimer;
  // unsigned long lastUpdateTime;
};

#endif
