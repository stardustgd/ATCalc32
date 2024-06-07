#include "calculator.h"
#include "avr.h"
#include "keypad.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void clearInput(char *buf, uint8_t *entries);
static void getInput(char *buf, uint8_t *entries, uint8_t x);

void inputHandler(CalcState *calcState) {
  switch (getKey()) {
  case A:
    *calcState = MODE_CALCULATE;
    break;
  case B:
    *calcState = MODE_QUIZ;
    break;
  }
}
void updateCalc(CalcState *calcState) {
  switch (*calcState) {
  case MODE_CALCULATE:
    // do something
    lcd_clr();
    lcd_pos(0, 0);
    calculatorMode();
    break;
  case MODE_QUIZ:
    // do quiz
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("quiz");
    break;
  }
}

void calculatorMode() {
  uint8_t entries = 0;
  uint8_t blink = 0;
  char inputOne[17] = {'\0'};
  char inputTwo[17] = {'\0'};

  getInput(inputOne, &entries, 0);
  lcd_pos(1, 0);
  getInput(inputTwo, &entries, 1);
}

static void clearInput(char *buf, uint8_t *entries) {
  buf[0] = '\0';
  *entries = 0;
  lcd_clr();
}

static void getInput(char *buf, uint8_t *entries, uint8_t x) {
  uint8_t valid = 0;
  uint8_t input;

  while (!valid) {
    wait_ms(200);

    input = getKey();

    if (input != NONE && input != STAR && *entries < 15) {
      sprintf(buf + strlen(buf), "%c%c", getKeyCode(input), '\0');
      lcd_pos(x, *entries);
      lcd_put(buf[*entries]);
      (*entries)++;
    }

    if (input == STAR) {
      clearInput(buf, entries);
    }

    if (input == A) {
      valid = 1;
      *entries = 0;
    }
  }
}
