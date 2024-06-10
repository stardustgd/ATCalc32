#ifndef QUIZ_H
#define QUIZ_H

#include "calculator.h"
#include "stdint.h"

#define NUM_QUESTIONS 10
#define MAX_NUMS 3

typedef struct {
  uint8_t nums[MAX_NUMS];
  char operations[MAX_NUMS - 1];
  char output[9];
  long long answer;
  uint8_t problemLength;
  uint8_t problemNum;
} MathProblem;

void quizStart(CalcState *calcState);
void displayQuestion(MathProblem question);
void validateQuestion(MathProblem question, uint8_t *userPoints);
void formatProblem(MathProblem *problem);
MathProblem generateQuestion(uint8_t problemNum);

#endif
