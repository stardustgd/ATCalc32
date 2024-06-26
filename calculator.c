#include "calculator.h"
#include "avr.h"
#include "keypad.h"
#include "lcd.h"
#include "quiz.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_INPUTS 32

static void clearInput(char *buf);
static void getInput(char *buf, CalcState *calcState);
static bool validateInput(char *buf);
static void calculateInput(char *buf);
static long long evaluateArithmetic(long long a, long long b, char operation);

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
    cursor_blink_on();
    lcd_clr();
    lcd_pos(0, 0);
    calculatorMode(calcState);
    break;
  case MODE_QUIZ:
    cursor_blink_off();
    quizStart(calcState);
    wait_ms(200);
    break;
  }
}

void calculatorMode(CalcState *calcState) {
  char input[MAX_INPUTS + 1] = {'\0'};

  getInput(input, calcState);
}

static void clearInput(char *buf) {
  buf[0] = '\0';
  lcd_clr();
}

static void getInput(char *buf, CalcState *calcState) {
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
        sprintf(buf + strlen(buf), "%s", "+\0");
        break;
      case B:
        sprintf(buf + strlen(buf), "%s", "-\0");
        break;
      case C:
        sprintf(buf + strlen(buf), "%s", "*\0");
        break;
      case D:
        sprintf(buf + strlen(buf), "%s", "/\0");
        break;
      default:
        sprintf(buf + strlen(buf), "%c%c", getKeyCode(input), '\0');
        break;
      }

      lcd_pos_and_put(buf[entries], lcd_y, lcd_x);
      entries++;
      lcd_x++;
    }

    if (input == POUND) {
      if (strcmp(buf, "+-*/") == 0) {
        *calcState = MODE_QUIZ;
        valid = true;
      } else if (validateInput(buf)) {
        lcd_clr();
        calculateInput(buf);
        entries = strlen(buf);
        lcd_x = entries % 16;
        lcd_y = (entries > 16) ? 1 : 0;
        wait_ms(500);
      } else {
        lcd_clr();
        lcd_pos_and_puts("Invalid Input", 0, 0);
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

      isLastOperation = true;

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
  char expression[MAX_INPUTS] = {'\0'};

  // Convert expression to postfix notation
  convertToPostfix(buf, expression);

  // After conversion, evaulate the expression
  long result = evaluateExpression(expression);

  // Output to LCD
  char tmp[MAX_INPUTS + 1] = {'\0'};
  sprintf(tmp + strlen(tmp), "%ld", result);
  lcd_pos_and_puts(tmp, 0, 0);
  lcd_pos(0, strlen(tmp));

  // Copy result into buf
  for (uint8_t i = 0; i < MAX_INPUTS; i++) {
    buf[i] = tmp[i];
  }
}

static uint8_t precedence(char operation) {
  if (operation == '+' || operation == '-') {
    return 1;
  }
  if (operation == '*' || operation == '/') {
    return 2;
  }
  return 0;
}

void convertToPostfix(const char *infix, char *postfix) {
  char stack[MAX_INPUTS * 2];
  int8_t stackTopIndex = -1;
  uint8_t resultIndex = 0;
  uint8_t infixLength = strlen(infix);

  for (uint8_t i = 0; i < infixLength; i++) {
    char c = infix[i];

    if (isdigit(c)) {
      while (isdigit(infix[i])) {
        postfix[resultIndex++] = infix[i++];
      }
      postfix[resultIndex++] = ' ';
      i--;
    } else {
      while (stackTopIndex != -1 &&
             precedence(stack[stackTopIndex]) >= precedence(infix[i])) {
        postfix[resultIndex++] = stack[stackTopIndex--];
        postfix[resultIndex++] = ' ';
      }
      stack[++stackTopIndex] = infix[i];
    }
  }

  while (stackTopIndex != -1) {
    postfix[resultIndex++] = stack[stackTopIndex--];
    postfix[resultIndex++] = ' ';
  }

  postfix[resultIndex] = '\0';
}

static long long evaluateArithmetic(long long a, long long b, char operation) {
  switch (operation) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/':
    return a / b;
  }

  return 0;
}

long long evaluateExpression(const char *postfix) {
  long long stack[MAX_INPUTS];
  int8_t stackTopIndex = -1;
  uint8_t postfixLength = strlen(postfix);

  for (uint8_t i = 0; i < postfixLength; i++) {
    if (isdigit(postfix[i])) {
      long long num = 0;
      while (isdigit(postfix[i])) {
        num = num * 10 + (postfix[i++] - '0');
      }

      stack[++stackTopIndex] = num;
    } else if (postfix[i] != ' ') {
      long long num2 = stack[stackTopIndex--];
      long long num1 = stack[stackTopIndex--];
      stack[++stackTopIndex] = evaluateArithmetic(num1, num2, postfix[i]);
    }
  }

  return stack[stackTopIndex];
}
