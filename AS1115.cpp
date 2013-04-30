/*
  AS1115.cpp - Arduino library for the austriamicrosystems AS1115 LED driver - implementation
  Copyright (c) 2011 Strich Labs  Licensed under Creative Commons BY-NC-SA.
*/

// include this library's description file
#include "AS1115.h"

// include description files for Wire, as the chip uses I2C
#include "Wire.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

// Description: Creates a variable of type AS1115, to communicate with the chip.
// Syntax: AS1115(address)
// Parameter: address - Address on the I2C bus of the chip to communicate with
// Returns: Instance of AS1115 associated with a specific chip.
AS1115::AS1115(byte chipAddress) {
  addr = chipAddress;
  cur_font = FONT_CODEB;
  Wire.begin();
}

AS1115::AS1115(void) {
  // this 'fake constructor' allows creating an 'empty' AS1115 type variable
  // later on, the 'real constructor' can be called to actually begin using it.
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Arduino sketches, this library, and other libraries

// Description: Configure the specified digit to be font-decoded or raw
// Syntax: AS1115Instance.setDigitDecode(digit, decode);
// Parameter: digit - Digit to modify
// Parameter: decode - DECODE_FONT or DECODE_RAW
// Returns: nothing
void AS1115::setDigitDecode(byte digit, byte decode) {
  as1115WriteRegisterBit(REG_DECODE_MODE, digit, decode);
}

// Description: Configure the intensity of the specified digit (or overall display)
// Syntax: AS1115Instance.setIntensity(digit, decode);
// Parameter: digit - Digit to modify (or INTENSITY_GLOBAL)
// Parameter: intensity - New intensity (0-255)
// Returns: nothing
void AS1115::setIntensity(byte digit, byte intensity) {
  byte regBuf;
  regBuf = as1115ReadRegister(REG_DIGIT01_INTEN + digit/2);
  regBuf = (regBuf & (digit % 2 == 0 ? 0x0F : 0xF0)) | map(intensity, 0, 255, 0, 15);
  as1115WriteRegister(REG_DECODE_MODE, regBuf);
}

// Description: Configure the display font (Code B or HEX)
// Syntax: AS1115Instance.setFont(font);
// Parameter: font - FONT_CODEB or FONT_HEX
// Returns: nothing
void AS1115::setFont(byte font) {
  as1115WriteRegisterBit(REG_FEATURE, REG_FEATURE_FONT, font);
  cur_font = font;
}

// Description: Write a digit (or series of 8 pixels) to the display
// Syntax: AS1115Instance.digitWrite(digit, value);
// Parameter: digit - Digit to modify
// Parameter: value - Value for the digit
// Returns: nothing
void AS1115::digitWrite(byte digit, byte value) {
  byte regBuf;
  if(cur_font == FONT_CODEB) {
    switch(value) {
      case '-':
        value = 0x0A; break;
      case 'E':
        value = 0x0B; break;
      case 'H':
        value = 0x0C; break;
      case 'L':
        value = 0x0D; break;
      case 'P':
        value = 0x0E; break;
      case ' ':
        value = 0x0F; break;
    }
  } else {
    switch(value) {
      case 'a':
      case 'A':
        value = 0x0A; break;
      case 'b':
      case 'B':
        value = 0x0B; break;
      case 'c':
      case 'C':
        value = 0x0C; break;
      case 'd':
      case 'D':
        value = 0x0D; break;
      case 'e':
      case 'E':
        value = 0x0E; break;
      case 'f':
      case 'F':
        value = 0x0F; break;
    }
  }
  as1115WriteRegister(REG_DIGIT0 + digit, value);
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
byte AS1115::as1115WriteRegister(byte reg, byte val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission();
}

byte AS1115::as1115ReadRegister(byte reg) {
  byte data;
  
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)addr, (uint8_t)1);
  data = Wire.read();
  return data;
}

byte AS1115::as1115WriteRegisterBit(byte reg, byte bit, byte value) {
  byte regBuf;
  regBuf = as1115ReadRegister(reg);
  bitWrite(regBuf, bit, value);
  return as1115WriteRegister(REG_DECODE_MODE, regBuf);
}
