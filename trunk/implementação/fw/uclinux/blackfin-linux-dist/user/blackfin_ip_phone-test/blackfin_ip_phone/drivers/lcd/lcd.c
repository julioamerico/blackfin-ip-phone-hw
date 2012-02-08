#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcd.h"

void lcd_init(void)
{
  drv_lcd_open();
  drv_lcd_init();

  drv_lcd_cursor(LCD_TOGGLE_OFF);
  drv_lcd_cursor_blink(LCD_TOGGLE_OFF);
}

int lcd_write_justified(lcd_write_justified_t lcd_op, int row, char str[])
{
	int col, col_end, len;
	
	len = strlen(str);

	switch (lcd_op)
	{
		case LCD_WRITE_LEFT_JUSTIFIED:
				col = 1;
				col_end = len;
				break;
		case LCD_WRITE_RIGHT_JUSTIFIED:
				col_end = LCD_MAX_COL;
				col = LCD_MAX_COL - len + 1;
				break;
		case LCD_WRITE_CENTER_JUSTIFIED:
				col = LCD_MAX_COL - len - (len % 2);
				col = col >> 1;
				col = col + 1 + (len % 2);
				col_end = col + len - 1;
				break;
		default:
				printf("Justify text option doesn't exist!\n");
				return 1;
	}

  if ((row < 1 || row > LCD_MAX_ROW) || (col < 1 || col > LCD_MAX_COL) || (col_end < 1 || col_end > LCD_MAX_COL))
  {
		printf("LCD can't handle this...\n");
    return 1;
  }
	else
	{
		drv_lcd_write(row, col, str, len);
		return 0;
	}
}

void lcd_clear_line(int line)
{
}
