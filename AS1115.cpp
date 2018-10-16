/*
  AS1115.cpp - Arduino library for the austriamicrosystems AS1115 LED driver - implementation
  Copyright (c) 2011 Strich Labs  Licensed under Creative Commons BY-NC-SA.
*/

// include this library's description file
#include "AS1115.h"

// include description files for Wire, as the chip uses I2C
#include "Wire.h"
#if defined(__AVR_ATmega328PB__)
  #define WIRE1_SUPPORT
	#include "Wire1.h"
#endif

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

// Description: Creates a variable of type AS1115, to communicate with the chip via a specific Wire number.
// Syntax: AS1115(address, wirenbr)
// Parameter: address - Address on the I2C bus of the chip to communicate with
// Parameter: wirenbr - Number of the wire library to use
// Returns: Instance of AS1115 associated with a specific chip.
AS1115::AS1115(byte chipAddress, byte wireNbr) {
  addr = chipAddress;
  cur_font = FONT_HEX;
  wire_nbr = wireNbr;
  if(wire_nbr == 0) {
    Wire.begin();
#ifdef WIRE1_SUPPORT
	} else if(wire_nbr == 1) {
    Wire1.begin();
#endif
	} else {
    return;
	}
}

// Description: Creates a variable of type AS1115, to communicate with the chip.
// Syntax: AS1115(address)
// Parameter: address - Address on the I2C bus of the chip to communicate with
// Returns: Instance of AS1115 associated with a specific chip.
AS1115::AS1115(byte chipAddress) {
  addr = chipAddress;
  cur_font = FONT_HEX;
  wire_nbr = 0;
  Wire.begin();
}

AS1115::AS1115(void) {
  // this 'fake constructor' allows creating an 'empty' AS1115 type variable
  // later on, the 'real constructor' can be called to actually begin using it.
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Arduino sketches, this library, and other libraries

// Description: Initialize the AS1115 chip
// Syntax: AS1115Instance.begin();
// Parameter: none
// Returns: nothing
void AS1115::begin(void) {
  // start up any chips at 0x00
  as1115WriteRegister(REG_SHUTDOWN, REG_SHUTDOWN_RUNNING | REG_SHUTDOWN_PRESERVE_FEATUREREG, 0x00);
  // ask all chips on the bus at 0x00 to use the strapped address, not the factory 0x00
  as1115WriteRegister(REG_SELF_ADDR, 0x01, 0x00);
  delay(20);
  
  // reset this chip and start it up
  as1115WriteRegister(REG_SHUTDOWN, REG_SHUTDOWN_RUNNING | REG_SHUTDOWN_RESET_FEATUREREG);  
  
  // display all digits, full brightness, decoded using the hex font
  as1115WriteRegister(REG_SCAN_LIMIT, 0x07);
  setIntensity(0xFF);
  setDecode(DECODE_ALL_FONT);
  setFont(FONT_HEX);
}

// Description: Configure the specified digit to be font-decoded or raw
// Syntax: AS1115Instance.setDecode(digit, decode);
// Parameter: digit - Digit to modify
// Parameter: decode - DECODE_FONT or DECODE_RAW
// Returns: nothing
void AS1115::setDecode(byte digit, byte decode) {
  as1115WriteRegisterBit(REG_DECODE_MODE, digit, decode);
}

// Description: Configure all digits at once to be font-decoded or raw
// Syntax: AS1115Instance.setDecode(decode);
// Parameter: decode - Bitmap of 8x DECODE_FONT or DECODE_RAW
// Returns: nothing
void AS1115::setDecode(byte decode) {
  as1115WriteRegister(REG_DECODE_MODE, decode);
}

// Description: Configure the intensity of the entire display
// Syntax: AS1115Instance.setIntensity(intensity);
// Parameter: intensity - New intensity (0-255)
// Returns: nothing
void AS1115::setIntensity(byte intensity) {
  as1115WriteRegister(REG_GLOBAL_INTEN, map(intensity, 0, 255, 0, 15));
}

// Description: Configure the intensity of the specified digit (or overall display)
// Syntax: AS1115Instance.setIntensity(digit, intensity);
// Parameter: digit - Digit to modify (or INTENSITY_GLOBAL)
// Parameter: intensity - New intensity (0-255)
// Returns: nothing
void AS1115::setIntensity(byte digit, byte intensity) {
  byte regBuf;
  byte regAddr = REG_DIGIT01_INTEN + digit/2;
  regBuf = as1115ReadRegister(regAddr);
  regBuf = (regBuf & (digit % 2 == 0 ? 0x0F : 0xF0)) | map(intensity, 0, 255, 0, 15);
  as1115WriteRegister(regAddr, regBuf);
}

// Description: Configure the display font (Code B or HEX)
// Syntax: AS1115Instance.setFont(font);
// Parameter: font - FONT_CODEB or FONT_HEX
// Returns: nothing
void AS1115::setFont(byte font) {
  as1115WriteRegisterBit(REG_FEATURE, REG_FEATURE_FONT, font);
  cur_font = font;
}

// Description: Turn on/off test mode (all digits on)
// Syntax: AS1115Instance.testMode(void);
// Parameters: onoff - 1 for test mode on, 0 for test mode off
// Returns: nothing
void AS1115::testMode(byte onoff) {
  as1115WriteRegister(REG_DISP_TEST, onoff);
}

// Description: Write a digit (or series of 8 pixels) to the display
// Syntax: AS1115Instance.digitWrite(digit, value);
// Parameter: digit - Digit to modify
// Parameter: value - Value for the digit
// Returns: nothing
void AS1115::digitWrite(byte digit, byte value) {
  digitWrite(digit, value, 0x00);
}

// Description: Write a digit (or series of 8 pixels) to the display
// Syntax: AS1115Instance.digitWrite(digit, value, dp);
// Parameter: digit - Digit to modify
// Parameter: value - Value for the digit
// Parameter: dp - Decmal point on/off
// Returns: nothing
void AS1115::digitWrite(byte digit, byte value, byte dp) {
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
  if(dp == DP_ON) value |= 0x80;
  as1115WriteRegister(REG_DIGIT0 + digit, value);
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
byte AS1115::as1115WriteRegister(byte reg, byte val, byte write_addr) {
  /*
  Serial.print("Writing 0x");
  Serial.print(val, HEX);
  Serial.print(" to register 0x");
  Serial.print(reg, HEX);
  Serial.print(" on address 0x");
  Serial.print(write_addr, HEX);
  Serial.print(" using Wire");
  Serial.println(wire_nbr, DEC);
  */
  if(wire_nbr == 0) {
    Wire.beginTransmission(write_addr);
    Wire.write(reg);
    Wire.write(val);
    return Wire.endTransmission();
#ifdef WIRE1_SUPPORT
  } else {
    Wire1.beginTransmission(write_addr);
    Wire1.write(reg);
    Wire1.write(val);
    return Wire1.endTransmission();
  }
#endif
}

byte AS1115::as1115WriteRegister(byte reg, byte val) {
  as1115WriteRegister(reg, val, addr);
}

byte AS1115::as1115ReadRegister(byte reg) {
  byte data;
  
  if(wire_nbr == 0) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)addr, (uint8_t)1);
    data = Wire.read();
#ifdef WIRE1_SUPPORT
  } else {
    Wire1.beginTransmission(addr);
    Wire1.write(reg);
    Wire1.endTransmission();
    Wire1.requestFrom((uint8_t)addr, (uint8_t)1);
    data = Wire1.read();
#endif
	}
  //Serial.print("Read 0x");
  //Serial.print(data, HEX);
  //Serial.print(" from register 0x");
  //Serial.println(reg, HEX);
  return data;
}

uint16_t AS1115::ReadKeysMul()
{
	uint8_t  Data1=as1115ReadRegister(REG_KEYA);
	uint8_t  Data2=as1115ReadRegister(REG_KEYB);	
	return ((unsigned int)Data1 << 8) + Data2;
}


byte AS1115::as1115WriteRegisterBit(byte reg, byte bit, byte value) {
  byte regBuf;
  regBuf = as1115ReadRegister(reg);
  bitWrite(regBuf, bit, value);
  return as1115WriteRegister(reg, regBuf);
}
