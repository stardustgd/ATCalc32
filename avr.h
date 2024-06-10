#ifndef _AVR_H
#define _AVR_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define XTAL_FRQ 8000000lu

#define SET_BIT(p, i) ((p) |= (1 << (i)))
#define CLR_BIT(p, i) ((p) &= ~(1 << (i)))
#define GET_BIT(p, i) ((p) & (1 << (i)))

#define WDR() asm volatile("wdr" ::)
#define NOP() asm volatile("nop" ::)
#define RST()                                                                  \
  for (;;)                                                                     \
    ;

void avr_init(void);

void seed_random(void);
void wait_ms(unsigned short msec);
void wait_us(unsigned short usec);

#endif /* _AVR_H */
