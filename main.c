#include "avr.h"
#include "calculator.h"

int main() {
  avr_init();
  CalcState calcState = MODE_CALCULATE;

  while (1) {
    inputHandler(&calcState);
    updateCalc(&calcState);
  }
}
