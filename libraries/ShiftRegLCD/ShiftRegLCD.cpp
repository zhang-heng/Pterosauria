/*

ShiftRegLCD  - Shiftregister-based HD44780 compatible LCD library for Arduino

Connects a Hitachi HD44780 compatible LCD using 2 or 3 pins from the Arduino,
via an 8-bit shift register (SR from now on).


Copyright (C) 2009-2012 raron

GNU GPLv3 license
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


  Contact: raronzen@gmail.com   (Not always checked)

  Project homepage with schematics:
  http://code.google.com/p/arduinoshiftreglcd/


Acknowledgements:
  Based very much on the "official" LiquidCrystal library for the Arduino:
    http://arduino.cc/en/Reference/Libraries
  And also the improved version (with examples CustomChar1 and SerialDisplay) from LadyAda:
    http://web.alfredstate.edu/weimandn/arduino/LiquidCrystal_library/LiquidCrystal_index.html
  And inspired by this schematics from an unknown author
    (thanks to mircho on the arduino playground forum!):
    http://www.scienceprog.com/interfacing-lcd-to-atmega-using-two-wires/
  And FINALLY a proper initialization routine for HD44780 compatible LCD's from
    http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html



Modified to work serially with an 8-bit shift register and an LCD.
2-wire and 3-wire mode can use either a latched or unlatched shiftregister.
  (Preferably, though not neccessarily, unlatched for 2-wire mode)



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


History
2012.02.15 raron - Fixed clear() and home() delay timings.
2012.02.09 raron - Final fixup on this version; a proper LCD initialization! 
2012.02.07 raron - Cleaned up setCursor(), but no 16x4 yet. For that see
           the next version of this library (ShiftRegLCD123).
2012.01.31 raron - Updated for Arduino 1.0, thanks to suggestions
           from florian.fida. Also (I think) debugged setCursor method, but
           haven't coped for 16x4 LCD's yet since I don't specify LCD size.
2011.07.02 raron - Fixed a minor flaw in setCursor function. No functional
           change, just a bit more memory efficient.
           Thanks to CapnBry (from google code and github) who noticed it.
           URL to his version of shiftregLCD:
           https://github.com/CapnBry/HeaterMeter/commit/c6beba1b46b092ab0b33bcbd0a30a201fd1f28c1
2009.07.30 raron - minor corrections to the comments.
           Fixed timing to datasheet safe. Fixed keyword highlights.
2009.07.28 Mircho / raron - a new modification to the schematics, and a
           more streamlined interface
2009.07.27 raron - Thanks to an excellent suggestion from mircho from Arduino 
           playground forum, the number of wires now required is only two!
2009.07.25 raron - Fixed comments. I really messed up the comments before 
           posting this, so I had to fix it.
           Renamed a function, but no improvements or functional changes.
2009.07.23 raron - Incorporated some proper initialization routines
           inspired (lets say copy-paste-tweaked) from LiquidCrystal
           library improvements from LadyAda.
2009.05.23 raron - first version, but based mostly (as in almost verbatim)
           on the "official" LiquidCrystal library.
*/

#include "ShiftRegLCD.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Assuming 1 line 8 pixel high font
ShiftRegLCD::ShiftRegLCD(uint8_t srdata, uint8_t srclock, uint8_t enable)
{
	init(srdata, srclock, enable, 1, 0);
}
// Set nr. of lines, assume 8 pixel high font
ShiftRegLCD::ShiftRegLCD(uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t lines)
{
	init(srdata, srclock, enable, lines, 0);
}
// Set nr. of lines and font
ShiftRegLCD::ShiftRegLCD(uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t lines, uint8_t font)
{
	init(srdata, srclock, enable, lines, font);
}


void ShiftRegLCD::init(uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t lines, uint8_t font)
{
  _two_wire    = 0;
  _srdata_pin  = srdata;
  _srclock_pin = srclock;
  _enable_pin  = enable;
  _numlines    = lines;
  _backlight   = 0;      // Backlight default off

  if (enable == TWO_WIRE)
  {
  	_enable_pin = _srdata_pin;
  	_two_wire = 1;
  }
  
  pinMode(_srclock_pin, OUTPUT);
  pinMode(_srdata_pin, OUTPUT);
  pinMode(_enable_pin, OUTPUT);

  digitalWrite(_srclock_pin, LOW);
  digitalWrite(_srdata_pin, LOW);
  digitalWrite(_enable_pin, LOW);

  if (lines>1)
  	_displayfunction = LCD_4BITMODE | LCD_2LINE;
  else
  	_displayfunction = LCD_4BITMODE | LCD_1LINE;


  // For some 1-line displays you can select a 10 pixel high font
  if (font != 0 && lines == 1)
    _displayfunction |= LCD_5x10DOTS;
  else
    _displayfunction |= LCD_5x8DOTS;

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 
  // more.
  // Also check this -excellent- guide on HD44780-based LCD initializations:
  // http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
  // (From which this initialization and comments are taken from)
  // ---------------------------------------------------------------------------

  // Step 1.   Power on, then delay > 100 ms
  
  // For some reason, delay() breaks execution of the rest of this library here
  //delay(200);
  for (int i=0; i<200; ++i) delayMicroseconds(1000);  // Waits 200 ms just in case.
  
  // Step 2.   Instruction 0011b (3h), then delay > 4.1 ms 
  init4bits(LCD_FUNCTIONSET | LCD_8BITMODE);
  delayMicroseconds(4500);  // wait more than 4.1ms
  
  // Step 3.   Instruction 0011b (3h), then delay > 100 us
  //   NOTE: send()/command() and init4bits() functions adds 60 us delay
  init4bits(LCD_FUNCTIONSET | LCD_8BITMODE);
  delayMicroseconds(40);
  
  // Step 4.   Instruction 0011b (3h), then delay > 100 us 
  init4bits(LCD_FUNCTIONSET | LCD_8BITMODE);
  delayMicroseconds(40);

  // Step 5.   Instruction 0010b (2h), then delay > 100 us (speculation)
  //   This is where display is set to 4-bit interface
  init4bits(LCD_FUNCTIONSET | LCD_4BITMODE);
  delayMicroseconds(40);
  
  // Step 6.   Instruction 0010b (2h), then 1000b (8h), then delay > 53 us or check BF 
  //   Set nr. of logical lines (not neccesarily physical lines) and font size
  //   (usually 2 lines and small font)
  command(LCD_FUNCTIONSET | _displayfunction);
  //delayMicroseconds(60);

  // Step 7.   Instruction 0000b (0h), then 1000b (8h) then delay > 53 us or check BF
  command(LCD_DISPLAYCONTROL);
  //delayMicroseconds(60);

  // Step 8.   Instruction 0000b (0h), then 0001b (1h) then delay > 3 ms or check BF 
  //   This is a Clear display instruction and takes a lot of time
  command(LCD_CLEARDISPLAY);
  delayMicroseconds(3000);
  
  // Step 9.   Instruction 0000b (0h), then 0110b (6h), then delay > 53 us or check BF 
  //   Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  command(LCD_DISPLAYCONTROL | _displaymode);
  //delayMicroseconds(60);

  // Step 10. Not really a step, but initialization has ended. Except display is turned off.
  
  // Step 11.   Instruction 0000b (0h), then 1100b (0Ch), then delay > 53 us or check BF
  //   Turn display on, no cursor, no cursor blinking.
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
  //delayMicroseconds(60);
}


// For sending data via the shiftregister
void ShiftRegLCD::send(uint8_t value, uint8_t mode)
{
  uint8_t val1, val2;
  if (_two_wire) shiftOut(_srdata_pin, _srclock_pin, MSBFIRST, 0x00); // clear shiftregister
  mode = mode ? SR_RS_BIT : 0; // RS bit; LOW: command.  HIGH: character.
  mode = _backlight ? mode | SR_BACKLIGHT : mode;  // Backlight
  val1 = mode | SR_EN_BIT | ((value >> 1) & 0x78); // upper nibble
  val2 = mode | SR_EN_BIT | ((value << 3) & 0x78); // lower nibble
  shiftOut(_srdata_pin, _srclock_pin, MSBFIRST, val1);
  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(1);                 // enable pulse must be >450ns
  digitalWrite( _enable_pin, LOW );
  if (_two_wire) shiftOut(_srdata_pin, _srclock_pin, MSBFIRST, 0x00); // clear shiftregister
  shiftOut(_srdata_pin, _srclock_pin, MSBFIRST, val2);
  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(1);                 // enable pulse must be >450ns
  digitalWrite(_enable_pin, LOW);
  // commands need > 37us to settle (for normal LCD with clock = 270 kHz)
  // Since LCD's may be slow (190 kHz clock = 53 us), we'll wait 60 us
  delayMicroseconds(60);
}

// For sending data when initializing the display to 4-bit
void ShiftRegLCD::init4bits(uint8_t value)
{
  uint8_t val1;
  if (_two_wire ) shiftOut (_srdata_pin, _srclock_pin, MSBFIRST, 0x00); // clear shiftregister
  val1 = SR_EN_BIT | ((value >> 1) & 0x78);
  shiftOut(_srdata_pin, _srclock_pin, MSBFIRST, val1);
  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(1);                 // enable pulse must be >450ns
  digitalWrite(_enable_pin, LOW);
  // commands need > 37us to settle (for normal LCD with clock = 270 kHz)
  // Since LCD's may be slow (190 kHz clock = 53 us), we'll wait 60 us
 delayMicroseconds(60);
}


void ShiftRegLCD::command(uint8_t value)
{
  send(value, LOW);
}

#if defined(ARDUINO) && ARDUINO >= 100
size_t ShiftRegLCD::write(uint8_t value)
#else
void ShiftRegLCD::write(uint8_t value)
#endif
{
  send(value, HIGH);
}


// ********** high level commands, for the user! **********

// Backlight On / Off control
void ShiftRegLCD::backlightOn()
{
  _backlight = 1;
  // Just to force a write to the shift register
  command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
}

void ShiftRegLCD::backlightOff()
{
  _backlight = 0;
  // Just to force a write to the shift register
  command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
}

void ShiftRegLCD::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(3000);    // this command takes a long time!
}

void ShiftRegLCD::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(3000);  // this command takes a long time!
}

void ShiftRegLCD::setCursor(uint8_t col, uint8_t row)
{
  // TODO: include support for 16x4 LCD's
  const uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row >= _numlines )
    row = _numlines-1;    // we count rows starting w/0
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}


// Turn the display on/off (quickly)
void ShiftRegLCD::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void ShiftRegLCD::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void ShiftRegLCD::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void ShiftRegLCD::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void ShiftRegLCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void ShiftRegLCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void ShiftRegLCD::shiftLeft(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void ShiftRegLCD::shiftRight(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void ShiftRegLCD::shiftIncrement(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void ShiftRegLCD::shiftDecrement(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations with custom characters
void ShiftRegLCD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | location << 3);
  for (int i=0; i<8; i++) write(charmap[i]);
  command(LCD_SETDDRAMADDR); // Reset display to display text (from pos. 0)
}

