/*
  AS1115.h - Arduino library for the austriamicrosystems AS1115 LED driver - description
  Copyright (c) 2012 sen.  Licensed under Creative Commons BY-SA.
*/

// ensure this library description is only included once
#ifndef AS1115_h
#define AS1115_h

#if ARDUINO < 100
  #error This library requires Arduino 1.0 or later.  Please upgrade your development environment.
#else // ARDUINO < 100

// include core Arduino API
#include "Arduino.h"

#define REG_DIGIT0        0x01
#define REG_DIGIT1        0x02
#define REG_DIGIT2        0x03
#define REG_DIGIT3        0x04
#define REG_DIGIT4        0x05
#define REG_DIGIT5        0x06
#define REG_DIGIT6        0x07
#define REG_DIGIT7        0x08
  
#define REG_DECODE_MODE   0x09
#define REG_GLOBAL_INTEN  0x0A
#define REG_SCAN_LIMIT    0x0B
#define REG_SHUTDOWN      0x0C
#define REG_SELF_ADDR     0x0D
#define REG_FEATURE       0x0E
#define REG_DISP_TEST     0x0F

#define REG_DIGIT01_INTEN 0x10
#define REG_DIGIT23_INTEN 0x11
#define REG_DIGIT45_INTEN 0x12
#define REG_DIGIT67_INTEN 0x13

#define REG_DIGIT0_DIAG   0x14
#define REG_DIGIT1_DIAG   0x15
#define REG_DIGIT2_DIAG   0x16
#define REG_DIGIT3_DIAG   0x17
#define REG_DIGIT4_DIAG   0x18
#define REG_DIGIT5_DIAG   0x19
#define REG_DIGIT6_DIAG   0x1A
#define REG_DIGIT7_DIAG   0x1B

#define REG_KEYA          0x1C
#define REG_KEYB          0x1D

#define DECODE_RAW        0x00
#define DECODE_FONT       0x01

#define FONT_CODEB        0x00
#define FONT_HEX          0x01

// global intensity register is 6 below the digit registers
#define INTENSITY_GLOBAL  -6

#define REG_FEATURE_EXTCLK      0x00
#define REG_FEATURE_RESET       0x01
#define REG_FEATURE_FONT        0x02
#define REG_FEATURE_BLINK       0x04
#define REG_FEATURE_BLINKFREQ   0x05
#define REG_FEATURE_BLINKSYNC   0x06
#define REG_FEATURE_BLINKSTART  0x07

// library interface description
class AS1115 {
  // user-accessible "public" interface
  public:
    AS1115(byte);
    AS1115(void);
    void setDigitDecode(byte, byte);
    void setIntensity(byte);
    void setFont(byte);
    byte digitWrite(byte, byte);
  
  // library-accessible "private" interface
  private:
    byte addr;
    byte cur_font;
    byte as1115WriteRegister(byte, byte);
    byte as1115ReadRegister(byte);
    void as1115WriteRegisterBit(byte, byte, byte);
};

#endif // ARDUINO < 100
#endif // AS1115_h
