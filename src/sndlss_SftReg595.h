/*
 * 74HC595 Shift registers library
 * https://github.com/bcc6/Arduino.Library.SftReg595
 * Copyright (c) Ozzy Chiu
 * Licensed under the WTFPL license.
 */

#ifndef SNDLSS_SFTREG595_H
#define SNDLSS_SFTREG595_H

#include <Arduino.h>

static const int    LEDS_NUMBER_OF_CHIPS = 4;
static const int    LEDS_DATA_BITS = 8;
static const int    LEDS_NUMBERS = 32;
static const int8_t LEDS_MAP[32] = {
    // Mapping Dn (LED number in schematic) to bit_n, Value '-1' means skip
    // Dn    1   2   3   4   5   6   7   8
             0,  1,  2,  3,  4,  5,  6,  7,
    // Dn    9  10  11  12  13  14  15  16
             8,  9, 10, 11, 12, 13, 14, 15,
    // Dn   17  18  19  20  21  22  23  24
            16, 17, 18, 19, 20, 21, 22, 23,
    // Dn   25  26  27  28  29  30  31  32
            24, 25, 26, 27, 28, 29, 30, 31,
};


// 74HC595: 8-bit serial-in, serial or parallel-out shift register with output latches
class SftReg595 {
 public:
  /** Create a LEDs object
   * @param sh Shift register clock (LOW-to-HIGH edge-triggered)
   * @param st Storage register clock (LOW-to-HIGH edge-triggered)
   * @param ds Serial data output
   */
  SftReg595(int sh, int st, int ds) : _sh(sh), _st(st), _ds(ds) {
    // set pins as output
    pinMode(_sh, OUTPUT);
    pinMode(_st, OUTPUT);
    pinMode(_ds, OUTPUT);

    // set pins low
    digitalWrite(_sh, 0);
    digitalWrite(_st, 0);
    digitalWrite(_ds, 0);
  }

  /** Write
   * @param data Bit0-31 correspond to LED1-LED32, '1' mean ON
   */
  void write(uint32_t data) { _data = data; }

  /** Read
   * @return Bit0-31 correspond to LED1-LED32, '1' mean ON
   */
  uint32_t read() { return _data; }

  /** Update
   */
  void update() {
    // Data & Clock
    for (int i=(LEDS_NUMBER_OF_CHIPS*LEDS_DATA_BITS-1); i>=0; i--) {
      int bit_n = LEDS_MAP[i];
      uint8_t bitval;
      if (bit_n < 0) {
        // OFF
        bitval = 1;
      } else {
        // ON : OFF, Low active
        bitval = (_data & (1L << bit_n)) ? 0 : 1;
      }
      digitalWrite(_ds, bitval);
      __asm__("nop\n\t");
      digitalWrite(_sh, 1);
      __asm__("nop\n\t");
      digitalWrite(_ds, 0);
      digitalWrite(_sh, 0);
      __asm__("nop\n\t");
    }

    // Latch
    digitalWrite(_st, 1);
    __asm__("nop\n\t");
    digitalWrite(_st, 0);
  }

 private:
  int _sh;
  int _st;
  int _ds;
  uint32_t _data;
};

#endif