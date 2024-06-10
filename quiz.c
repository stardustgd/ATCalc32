#include "quiz.h"
#include "avr.h"
#include "keypad.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME_TO_SOLVE 10

const char OPERATORS[] = {'+', '-', '*'};

static uint8_t handleInput(char *input, uint8_t key, uint8_t *entries);
static void printTimer(uint8_t secs);
static void printResults(uint8_t userPoints);
static void showCorrect();
static void showIncorrect();

void quizStart(CalcState *calcState) {
  uint8_t userPoints = 0;
  lcd_clr();

  for (int8_t i = 0; i < 3; i++) {
    wait_ms(500);
    lcd_pos_and_puts("Quiz Time!", 0, 3);
    wait_ms(500);
    lcd_clr();
  }

  lcd_pos_and_puts("Quiz Time!", 0, 3);
  wait_ms(2000);

  for (uint8_t i = 0; i < NUM_QUESTIONS; i++) {
    MathProblem problem = generateQuestion(i + 1);
    displayQuestion(problem);
    validateQuestion(problem, &userPoints);
  }

  printResults(userPoints);
  *calcState = MODE_CALCULATE;
}

void validateQuestion(MathProblem problem, uint8_t *userPoints) {
  uint8_t input;
  uint8_t cycles = 0;
  uint8_t entries = 0;
  uint8_t secsLeft = TIME_TO_SOLVE;
  char buf[17] = {'\0'};

  while (secsLeft > 0) {
    wait_ms(200);
    printTimer(secsLeft);

    cycles++;

    // Check if 1s elapsed
    if (cycles == 5) {
      secsLeft--;
      cycles = 0;
    }

    input = getKey();

    // Check the code returned by the input handler
    uint8_t inputHandlerCode = handleInput(buf, input, &entries);

    if (inputHandlerCode == 1) {
      secsLeft = 0;
    } else if (inputHandlerCode == 2) {
      buf[0] = '\0';
      entries = 0;
      lcd_clr();
      displayQuestion(problem);
      printTimer(secsLeft);
    }
  }

  char tmp[16] = {'\0'};
  sprintf(tmp, "%ld", (long)problem.answer);

  if (strcmp(tmp, buf) == 0) {
    showCorrect();
    (*userPoints)++;
  } else {
    showIncorrect();
  }
}

static uint8_t handleInput(char *input, uint8_t key, uint8_t *entries) {
  if (key != NONE && key != POUND && key != A && key != C && key != D &&
      key != STAR && *entries < 16) {
    switch (key) {
    case B:
      sprintf(input + strlen(input), "%s", "-\0");
      break;
    default:
      sprintf(input + strlen(input), "%c%c", getKeyCode(key), '\0');
      break;
    }

    lcd_pos_and_put(input[*entries], 1, *entries);
    (*entries)++;
  } else if (key == STAR) {
    return 2;
  } else if (key == POUND) {
    return 1;
  }

  return 0;
}

static void printResults(uint8_t userPoints) {
  float grade = (float)userPoints / NUM_QUESTIONS;
  char tmp[16] = {'\0'};
  sprintf(tmp, "%2d/%d", userPoints, NUM_QUESTIONS);

  lcd_clr();
  lcd_pos_and_puts(tmp, 1, 5);

  if (grade < 0.2) {
    lcd_pos_and_puts("Are you trying?", 0, 0);
  } else if (grade < 0.5) {
    lcd_pos_and_puts("Good effort!", 0, 2);
  } else if (grade <= 0.9) {
    lcd_pos_and_puts("Nice job!!", 0, 3);
  } else if (grade == 1.0) {
    lcd_pos_and_puts("Perfect!!", 0, 4);
  }

  wait_ms(5000);
}

static void showCorrect() {
  lcd_clr();
  lcd_pos_and_puts("Correct!", 0, 4);
  wait_ms(1000);
}

static void showIncorrect() {
  lcd_clr();
  lcd_pos_and_puts("Incorrect!", 0, 3);
  wait_ms(1000);
}

static void printTimer(uint8_t secs) {
  char buf[5] = {'\0'};
  sprintf(buf, "%02d%c", secs, '\0');

  lcd_pos_and_puts(buf, 0, 14);
}

void displayQuestion(MathProblem question) {
  lcd_clr();
  char tmp[16] = {'\0'};
  sprintf(tmp, "%d. %s", question.problemNum, question.output);
  lcd_pos_and_puts(tmp, 0, 0);
}

MathProblem generateQuestion(uint8_t problemNum) {
  const uint8_t PROBLEM_LENGTH = rand() % (MAX_NUMS - 1) + 2;
  const uint8_t NUM_OPERATORS = PROBLEM_LENGTH - 1;

  MathProblem problem;
  problem.problemLength = PROBLEM_LENGTH;
  problem.problemNum = problemNum;

  // Randomly generate numbers
  for (uint8_t i = 0; i < PROBLEM_LENGTH; i++) {
    problem.nums[i] = rand() % 99 + 1;
  }

  for (uint8_t i = 0; i < NUM_OPERATORS; i++) {
    problem.operations[i] = OPERATORS[rand() % 3];
  }

  // Format the problem to infix as a string
  formatProblem(&problem);

  // Convert the problem to postfix, then evaluate
  char postfix[16];
  convertToPostfix(problem.output, postfix);
  problem.answer = evaluateExpression(postfix);

  return problem;
}

void formatProblem(MathProblem *problem) {
  problem->output[0] = '\0';

  for (uint8_t i = 0; i < problem->problemLength; i++) {
    char numBuf[16];

    sprintf(numBuf, "%d", problem->nums[i]);
    strcat(problem->output, numBuf);

    if (i < problem->problemLength - 1) {
      char opBuf[2] = {problem->operations[i], '\0'};
      strncat(problem->output, opBuf, 2);
    }
  }
}
