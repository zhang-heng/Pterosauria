// Example creating custom characters 0-7 on a HD44700 compatible LCD
// Using 2-wire connection to LCD

#include <ShiftRegLCD.h>

const byte dataPin  = 10;    // SR Data from Arduino pin 10
const byte clockPin = 11;    // SR Clock from Arduino pin 11

ShiftRegLCD srlcd(dataPin, clockPin, TWO_WIRE);

void setup()
{
  // Turn on backlight (if used)
  srlcd.backlightOn();

  // very simple characters, could be used for a bar-graph
  uint8_t customchar[][8] ={ { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F },
                         { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F },
                         { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F },
                         { 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F }, 
                         { 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F },
                         { 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F },
                         { 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F },
                         { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F } };


  // Program the characters into the LCD

  for (uint8_t c = 0;c<8;c++) {
      srlcd.createChar(c, customchar[c]);
  }

  for (uint8_t i=0;i<8;i++) srlcd.write(i);
  srlcd.print("CUSTOM CHARS");
  for (uint8_t i=8;i>0;i--) srlcd.write(i-1);
}

void loop()
{
}

