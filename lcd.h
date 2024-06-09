#ifndef _LCD_H
#define _LCD_H

void lcd_init(void);

void lcd_clr(void);

void lcd_pos(unsigned char r, unsigned char c);

void lcd_put(char c);

void lcd_puts1(const char *s);

void lcd_puts2(const char *s);

void lcd_pos_and_put(char ch, unsigned char r, unsigned char c);

void lcd_pos_and_puts(const char *s, unsigned char r, unsigned char c);

void cursor_blink_on();

void cursor_blink_off();

#endif /* _LCD_H */
