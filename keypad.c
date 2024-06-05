#include "keypad.h"
#include "avr.h"

#define DDR DDRC
#define PORT PORTC
#define PIN PINC

uint8_t isPressed(uint8_t r, uint8_t c) {
  // all 8 GPIOs set to N/C
  DDR = 0;

  c += 4;

  // set r to "0"
  SET_BIT(DDR, r);
  CLR_BIT(PORT, r);

  // set c to "w1"
  CLR_BIT(DDR, c);
  SET_BIT(PORT, c);

  return (!GET_BIT(PIN, c)) ? 1 : 0;
}

uint8_t getKey() {
  for (uint8_t i = 0; i < 4; i++) {
    for (uint8_t j = 0; j < 4; j++) {
      if (isPressed(i, j)) {
        return (i * 4) + (j + 1);
      }
    }
  }

  return 0;
}

char getKeyCode(uint8_t key) {
  switch (key) {
  case ONE:
    return '1';
  case TWO:
    return '2';
  case THREE:
    return '3';
  case FOUR:
    return '4';
  case FIVE:
    return '5';
  case SIX:
    return '6';
  case SEVEN:
    return '7';
  case EIGHT:
    return '8';
  case NINE:
    return '9';
  case ZERO:
    return '0';
  case A:
    return 'A';
  case B:
    return 'B';
  case C:
    return 'C';
  case D:
    return 'D';
  case STAR:
    return '*';
  case POUND:
    return '#';
  default:
    return ' ';
  }
}
