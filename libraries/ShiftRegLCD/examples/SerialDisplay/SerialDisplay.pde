// Displays text sent over the serial port (e.g. from the Serial Monitor) on
// an attached LCD, using 2-wire connection to the LCD.

#include <ShiftRegLCD.h>

const byte dataPin  = 10;    // SR Data from Arduino pin 10
const byte clockPin = 11;    // SR Clock from Arduino pin 11

ShiftRegLCD srlcd(dataPin, clockPin, TWO_WIRE);

void setup()
{
  Serial.begin(9600);

  // Turn on LCD backlight (if used)
  srlcd.backlightOn();
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    srlcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      srlcd.write(Serial.read());
    }
  }
}

