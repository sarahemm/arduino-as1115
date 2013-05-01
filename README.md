## AS1115()

### Description
Creates a variable of type AS1115, to communicate with the chip.

### Syntax
AS1115(*address*)

### Parameters
*address* - Address on the I2C bus of the chip to communicate with

### Returns
Instance of AS1115 associated with a specific chip.

### Example
    #include <Wire.h>
    #include <AS1115.h>
    
    // Create an AS1115 instance to communicate with the chip at address 0x00
    AS1115 display = AS1115(0x00);

## setDecode()

### Description
Configure the specified digit to be font-decoded or raw

### Syntax
*AS1115Instance*.setDecode(*digit*, *decode*);

### Parameters
*digit* - Digit to modify
*decode* - DECODE_FONT or DECODE_RAW

### Returns
nothing

### Example
    #include <Wire.h>
    #include <AS1115.h>
    
    // Create an AS1115 instance to communicate with the chip at address 0x00
    AS1115 display = AS1115(0x00);
    
    void setup() {
      // Set entire display to raw pixel mode, not using any decode
      display.setDecode(DECODE_ALL_RAW);
      
      // Set digit 4 to decode using a font, not RAW pixels
      display.setDecode(4, DECODE_RAW);
    }

## setDecode()

### Description
Configure all digits at once to be font-decoded or raw

### Syntax
*AS1115Instance*.setDecode(*decode*);

### Parameters
*decode* - Bitmap of 8x DECODE_FONT or DECODE_RAW

### Returns
nothing

### Example
    #include <Wire.h>
    #include <AS1115.h>
    
    // Create an AS1115 instance to communicate with the chip at address 0x00
    AS1115 display = AS1115(0x00);
    
    void setup() {
      // Set entire display to raw pixel mode, not using any decode
      display.setDecode(DECODE_ALL_RAW);
      
      // Set digit 4 to decode using a font, not RAW pixels
      display.setDecode(4, DECODE_RAW);
    }

## setIntensity()

### Description
Configure the intensity of the specified digit (or overall display)

### Syntax
*AS1115Instance*.setIntensity(*digit*, *decode*);

### Parameters
*digit* - Digit to modify (or INTENSITY_GLOBAL)
*intensity* - New intensity (0-255)

### Returns
nothing

### Example
    #include <Wire.h>
    #include <AS1115.h>
    
    // Create an AS1115 instance to communicate with the chip at address 0x00
    AS1115 display = AS1115(0x00);
    
    void setup() {
      // Set digit 2 to half-brightness
      display.setIntensity(2, 128);
    }

## setFont()

### Description
Configure the display font (Code B or HEX)

### Syntax
*AS1115Instance*.setFont(*font*);

### Parameters
*font* - FONT_CODEB or FONT_HEX

### Returns
nothing

### Example
    #include <Wire.h>
    #include <AS1115.h>
    
    // Create an AS1115 instance to communicate with the chip at address 0x00
    AS1115 display = AS1115(0x00);
    
    void setup() {
      // Set the font on the entire display to Code-B
      display.setFont(FONT_CODEB);
    }

## digitWrite()

### Description
Write a digit (or series of 8 pixels) to the display

### Syntax
*AS1115Instance*.digitWrite(*digit*, *value*);

### Parameters
*digit* - Digit to modify
*value* - Value for the digit

### Returns
nothing

### Example
    #include <Wire.h>
    #include <AS1115.h>
    
    // Create an AS1115 instance to communicate with the chip at address 0x00
    AS1115 display = AS1115(0x00);
    
    void setup() {
      // Set the font to HEX so we can display 0-F properly
      display.setFont(FONT_HEX);
      
      // Display a 9 on digit 0
      display.digitWrite(0, 9);
      
      // display an F on digit 1
      display.digitWrite(1, 0x0F);
      
      // display an A on digit 2
      display.digitWrite(2, 'A');
    }

