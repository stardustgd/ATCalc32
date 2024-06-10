#ifndef QUIZ_H
#define QUIZ_H

#include "stdint.h"

#define NUM_QUESTIONS 10
#define MAX_NUMS 3

typedef struct {
  uint8_t nums[MAX_NUMS];
  char operations[MAX_NUMS - 1];
  char output[16];
  long long answer;
  uint8_t problemLength;
} MathProblem;

void quizStart();
MathProblem generateQuestion();

#endif
