/************************************************************************************************************************************
 * ShiftPWM blocking RGB fades example, (c) Elco Jacobs, updated August 2012.
 *
 * ShiftPWM blocking RGB fades example. This example uses simple delay loops to create fades.
 * If you want to change the fading mode based on inputs (sensors, buttons, serial), use the non-blocking example as a starting point.
 * Please go to www.elcojacobs.com/shiftpwm for documentation, fuction reference and schematics.
 * If you want to use ShiftPWM with LED strips or high power LED's, visit the shop for boards.
 ************************************************************************************************************************************/

// ShiftPWM uses timer1 by default. To use a different timer, before '#include <ShiftPWM.h>', add
 #define SHIFTPWM_USE_TIMER2  // for Arduino Uno and earlier (Atmega328)
// #define SHIFTPWM_USE_TIMER3  // for Arduino Micro/Leonardo (Atmega32u4)

// Clock and data pins are pins from the hardware SPI, you cannot choose them yourself if you use the hardware SPI.
// Data pin is MOSI (Uno and earlier: 11, Leonardo: ICSP 4, Mega: 51, Teensy 2.0: 2, Teensy 2.0++: 22)
// Clock pin is SCK (Uno and earlier: 13, Leonardo: ICSP 3, Mega: 52, Teensy 2.0: 1, Teensy 2.0++: 21)

// You can choose the latch pin yourself.
const int ShiftPWM_latchPin = 7;

// ** uncomment this part to NOT use the SPI port and change the pin numbers. This is 2.5x slower **
// #define SHIFTPWM_NOSPI
// const int ShiftPWM_dataPin = 11;
// const int ShiftPWM_clockPin = 13;


// If your LED's turn on if the pin is low, set this to true, otherwise set it to false.
const bool ShiftPWM_invertOutputs = true;

// You can enable the option below to shift the PWM phase of each shift register by 8 compared to the previous.
// This will slightly increase the interrupt load, but will prevent all PWM signals from becoming high at the same time.
// This will be a bit easier on your power supply, because the current peaks are distributed.
const bool ShiftPWM_balanceLoad = false;

#include <ShiftPWM.h>   // include ShiftPWM.h after setting the pins!

// Here you set the number of brightness levels, the update frequency and the number of shift registers.
// These values affect the load of ShiftPWM.
// Choose them wisely and use the PrintInterruptLoad() function to verify your load.
// There is a calculator on my website to estimate the load.

unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
int numRegisters = 5;
int numRGBleds = numRegisters * 8 / 3; // Max number: numRegisters * 8 / 3

#include "LedEffectDefine.h"

/*
 BLE
 */

#include <AltSoftSerial.h>
// UNO RX: PIN8, TXPIN9
AltSoftSerial BLE;

/*
 functions
 */

int currentLed = 0;
int currentColor[11][3] = {{0}};

byte readBLE()
{
  delay(10);
  byte data = BLE.read();
  Serial.println(data);
  return data;
}

void turnAllLEDsOff()
{
  ShiftPWM.SetAll(0);
}

void changeAllLeds(int rgb[3])
{
  for (int i = 0; i < 11; i++) {
    currentColor[i][0] = rgb[0];
    currentColor[i][1] = rgb[1];
    currentColor[i][2] = rgb[2];
    ShiftPWM.SetRGB(i, rgb[0], rgb[1], rgb[2]);
  }
}

void pushOneLed(int *rgb)
{
  if (currentLed < maxLedNumber) {
    currentColor[currentLed][0] = rgb[0];
    currentColor[currentLed][1] = rgb[1];
    currentColor[currentLed][2] = rgb[2];
    ShiftPWM.SetRGB(LedList[currentLed], rgb[0], rgb[1], rgb[2]);
    currentLed++;
  } else {
    for (int i = maxLedNumber - 2; i >= 0; i--) {
      currentColor[i+1][0] = currentColor[i][0];
      currentColor[i+1][1] = currentColor[i][1];
      currentColor[i+1][2] = currentColor[i][2];
      ShiftPWM.SetRGB(LedList[i+1], currentColor[i+1][0], currentColor[i+1][1], currentColor[i+1][2]);
    }
    ShiftPWM.SetRGB(LedList[0], rgb[0], rgb[1], rgb[2]);
  }
}

void setup()
{
  // control LEDs by shift PWM
  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // SetPinGrouping allows flexibility in LED setup.
  // If your LED's are connected like this: RRRRGGGGBBBBRRRRGGGGBBBB, use SetPinGrouping(4).
  ShiftPWM.SetPinGrouping(1); //This is the default, but I added here to demonstrate how to use the funtion

  ShiftPWM.Start(pwmFrequency, maxBrightness);

  // access data by ble
  BLE.begin(57600);
  Serial.begin(57600);

  turnAllLEDsOff();
}

void loop()
{
  if (BLE.available()) {
    byte data = readBLE();
    if (data == kInit) {
      currentLed = 0;
      turnAllLEDsOff();
    } else if (data == kAllHappy) {
      changeAllLeds(HappyColor);
    } else if (data == kAllAngry) {
      changeAllLeds(AngryColor);
    } else if (data == kAllHelpless) {
      changeAllLeds(HelplessColor);
    } else if (data == kAllWorried) {
      changeAllLeds(WorriedColor);
    } else if (data == kAllNervous) {
      changeAllLeds(NervousColor);
    } else if (data == kAllExcited) {
      changeAllLeds(ExcitedColor);
    } else if (data == kAddHappy) {
      pushOneLed(HappyColor);
    } else if (data == kAddAngry) {
      pushOneLed(AngryColor);
    } else if (data == kAddHelpless) {
      pushOneLed(HelplessColor);
    } else if (data == kAddWorried) {
      pushOneLed(WorriedColor);
    } else if (data == kAddNervous) {
      pushOneLed(NervousColor);
    } else if (data == kAddExcited) {
      pushOneLed(ExcitedColor);
    }
  }
}
