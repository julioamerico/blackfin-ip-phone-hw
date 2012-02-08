/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	main.c - Main application file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drivers/lcd/drv_lcd.h"
#include "drivers/lcd/lcd.h"

int main (void)
{
	printf("O programa está sendo executado!!\n");

  lcd_init();

	drv_lcd_clear_screen();

	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 1, "XUXA M.");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 1, "1002");
  lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, "17:05");
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, "SUN, 10/05/2012");
	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 4, "MENU");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 4, "CONTACTS");

  drv_lcd_close();

	return 0;
}
