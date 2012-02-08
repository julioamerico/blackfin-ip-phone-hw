#include "drv_lcd.h"

#ifndef BLACKFIN_IP_PHONE_LCD_H
#define BLACKFIN_IP_PHONE_LCD_H

typedef enum {
	LCD_WRITE_LEFT_JUSTIFIED,
	LCD_WRITE_RIGHT_JUSTIFIED,
	LCD_WRITE_CENTER_JUSTIFIED,

	LCD_WRITE_NULL
} lcd_write_justified_t;

void lcd_init(void);
int lcd_write_justified(lcd_write_justified_t lcd_op, int row, char str[]);
void lcd_clear_line(int line);

#endif /* BLACKFIN_IP_PHONE_LCD_H */
