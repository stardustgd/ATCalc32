#include "avr.h"
#include "lcd.h"

int main() {
  avr_init();

  while (1) {
    lcd_pos(0, 0);
    lcd_puts2("Hello World");
  }
}
