#ifndef CALCULATOR_H
#define CALCULATOR_H

typedef enum { MODE_CALCULATE, MODE_QUIZ } CalcState;
extern CalcState calcState;

void inputHandler(CalcState *calcState);
void updateCalc(CalcState *calcState);
void calculatorMode();
void convertToPostfix(const char *infix, char *postfix);
long long evaluateExpression(const char *postfix);

#endif
