#include "calculator.h"
#include "avr.h"
#include "keypad.h"
#include "lcd.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUTS 32

static void clearInput(char *buf);
static void getInput(char *buf);
static bool validateInput(char *buf);
static void calculateInput(char *buf);

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
    lcd_clr();
    lcd_pos(0, 0);
    calculatorMode();
    break;
  case MODE_QUIZ:
    lcd_clr();
    lcd_pos(0, 0);
    lcd_puts2("quiz");
    wait_ms(200);
    break;
  }
}

void calculatorMode() {
  char input[MAX_INPUTS + 1] = {'\0'};

  getInput(input);
}

static void clearInput(char *buf) {
  buf[0] = '\0';
  lcd_clr();
}

static void getInput(char *buf) {
  bool valid = false;
  uint8_t entries = 0;
  uint8_t lcd_x = 0;
  uint8_t lcd_y = 0;
  uint8_t input;

  while (!valid) {
    wait_ms(200);
    input = getKey();

    if (input == STAR) {
      clearInput(buf);
      entries = 0;
      lcd_y = 0;
      lcd_x = 0;
    } else if (input != NONE && input != POUND && entries < MAX_INPUTS) {
      switch (input) {
      case A:
        sprintf(buf + strlen(buf), "%s", "+");
        break;
      case B:
        sprintf(buf + strlen(buf), "%s", "-");
        break;
      case C:
        sprintf(buf + strlen(buf), "%s", "*");
        break;
      case D:
        sprintf(buf + strlen(buf), "%s", "/");
        break;
      default:
        sprintf(buf + strlen(buf), "%c%c", getKeyCode(input), '\0');
        break;
      }

      lcd_pos(lcd_y, lcd_x);
      lcd_put(buf[entries]);
      entries++;
      lcd_x++;
    }

    if (input == POUND) {
      if (validateInput(buf)) {
        valid = true;
        lcd_clr();
        lcd_pos(0, 0);
        lcd_puts2("valid");
        wait_ms(500);
      } else {
        lcd_clr();
        lcd_pos(0, 0);
        lcd_puts2("Invalid input");
        wait_ms(1000);
        clearInput(buf);
        lcd_x = 0;
        lcd_y = 0;
        entries = 0;
      }
    }

    if (entries == (MAX_INPUTS / 2)) {
      lcd_x = 0;
      lcd_y = 1;
      lcd_pos(lcd_y, lcd_x);
    } else if (entries >= MAX_INPUTS) {
      lcd_pos(lcd_y, 15);
    }
  }
}

static bool validateInput(char *buf) {
  // Set to true if the last char was an operation
  bool isLastOperation = false;
  bool hasDigits = false;

  for (uint8_t i = 0; buf[i] != '\0'; i++) {
    char c = buf[i];

    if (isdigit(c)) {
      hasDigits = true;
      isLastOperation = false;
    } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '.') {
      if (isLastOperation) {
        return false;
      }

      // The first char is an operation
      if (i == 0) {
        return false;
      }
    }
  }

  // The last char is an operation
  if (isLastOperation) {
    return false;
  }

  if (!hasDigits) {
    return false;
  }

  return true;
}

static void calculateInput(char *buf) {
  // TODO: tokenize input and solve according to PEMDAS
}
