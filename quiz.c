#include "quiz.h"
#include <stdlib.h>
#include <time.h>

MathProblem generateQuestion() {
  MathProblem problem;

  srand(time(0));

  problem.num1 = rand() % 100;
  problem.num2 = rand() % 100;

  switch (rand() % 4) {
  case 0:
    problem.operation = '+';
    break;
  case 1:
    problem.operation = '-';
    break;
  case 2:
    problem.operation = '*';
    break;
  case 3:
    problem.operation = '/';
    break;
  }

  return problem;
}

// char *formatProblem(MathProblem problem) {
//   char buf[16];
//   sprintf(buf, "%d", problem.num1);
//
//   return buf;
// }
