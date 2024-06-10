#include "quiz.h"
#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char OPERATORS[] = {'+', '-', '*'};

void displayQuestion();
void formatProblem(MathProblem *problem);

void quizStart(CalcState *calcState) {
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
    displayQuestion();
  }

  *calcState = MODE_CALCULATE;
}

void displayQuestion() {
  MathProblem question = generateQuestion();

  char buf[16] = {'\0'};
  long ans = question.answer;
  sprintf(buf, "%ld", ans);

  lcd_clr();
  lcd_pos_and_puts(question.output, 0, 0);
  lcd_pos_and_puts(buf, 1, 0);
  wait_ms(5000);
  lcd_clr();
}

MathProblem generateQuestion() {
  const uint8_t PROBLEM_LENGTH = rand() % (MAX_NUMS - 1) + 2;
  const uint8_t NUM_OPERATORS = PROBLEM_LENGTH - 1;

  MathProblem problem;
  problem.problemLength = PROBLEM_LENGTH;

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
