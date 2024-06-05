#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

enum Key {
  NONE,
  ONE,
  TWO,
  THREE,
  A,
  FOUR,
  FIVE,
  SIX,
  B,
  SEVEN,
  EIGHT,
  NINE,
  C,
  STAR,
  ZERO,
  POUND,
  D
};

uint8_t isPressed(uint8_t r, uint8_t c);
uint8_t getKey();
char getKeyCode(uint8_t key);
char get_keypad_input();

#endif
