#include <Wire.h>
#include <AS1115.h>

AS1115 disp = AS1115(0x00);
byte counter = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.print("Initializing AS1115...");
  disp.begin();
  disp.setFont(FONT_HEX);
  disp.setDecode(DECODE_ALL_FONT);
  disp.digitWrite(0, 0);
  disp.digitWrite(1, 1);
  disp.digitWrite(2, 2);
  disp.digitWrite(3, 3);
  disp.digitWrite(4, 4);
  disp.digitWrite(5, 5);
  disp.digitWrite(6, 6);
  disp.digitWrite(7, 7);
  Serial.println("done.");
}

void loop(void) {
  byte digit;
  
  Serial.print("Displaying ");
  for(digit=0; digit<=7; digit++) {
    disp.digitWrite(digit, counter);
    Serial.print(counter, HEX);
  }
  Serial.println("");
  counter++;
  if(counter == 0x10) counter = 0;
  
  delay(500);
}