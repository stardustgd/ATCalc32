#ifndef QUIZ_H
#define QUIZ_H

#include "stdint.h"

typedef struct {
  uint8_t num1;
  uint8_t num2;
  char operation;
} MathProblem;

MathProblem generateQuestion();
char *formatQuestion(MathProblem problem);

#endif
