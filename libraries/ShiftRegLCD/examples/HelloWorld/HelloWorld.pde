/* 2-wire connection demo to a HD44780-compatible LCD via shiftregister with:
     Data (and Enable) on Arduino pin 10
     Clock on Arduino pin 11

CONNECTION DESCRIPTION

ShiftRegLCD

 Requires 2 or 3 pins from the Arduino.
 Arduino outputs to shift register data, clock and (optional) LCD Enable inputs.

 SR output:
  Bit  #0   - N/C - not connected. Must be 0 in 2-wire mode.
  Bit  #1   - LCD backlight control bit, or unused.
                Do not connect directly! Use a driver/ transistor!
  Bit  #2   - connects to RS (Register Select) on the LCD
  Bits #3-6 - connects to LCD data inputs D4 - D7.
  Bit  #7   - 2-wire mode: To cathode of the diode in the diode-resistor AND "gate"
              3-wire mode: Unused

  LCD RW-pin hardwired to LOW (only writing to LCD).
  Busy Flag (BF, data bit D7) is not read.


USAGE:

  ShiftRegLCD LCDobjectvariablename(Datapin, Clockpin, Enablepin or TWO_WIRE [, Lines [, Font]]])

  where:
    Lines    : Optional. Sets display nr of (logical) lines (1 or 2).
    Font     : Optional. Sets small (normal 5x8 pix) or large (5x10 pix) font.
    Enablepin: Can be replaced by constant TWO_WIRE, if using only 2 wires.
*/

#include <ShiftRegLCD.h>

const byte dataPin  = 10;    // SR Data from Arduino pin 10
const byte clockPin = 11;    // SR Clock from Arduino pin 11

ShiftRegLCD srlcd(dataPin, clockPin, TWO_WIRE);

void setup()
{
  // Turn on backlight (if used)
  srlcd.backlightOn();
  
  // Print a message to the LCD.
  srlcd.print("HELLO, WORLD!");
}

void loop()
{
}

