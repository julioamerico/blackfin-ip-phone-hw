/*
 * Project: BLACKFIN IP PHONE
 * 
 * lcd_test.c - LCD test application
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <strings.h>

#include "linux/i2c-dev.h"
#include "lcd_test.h"
#include "drv_lcd.h"

int main (void)
{
  char lcd_string[20];

  drv_lcd_open();
  drv_lcd_init();
 
  drv_lcd_cursor(LCD_TOGGLE_OFF);
  drv_lcd_cursor_blink(LCD_TOGGLE_OFF);

/*
  strcpy (lcd_string, "tEsTe aBcDeFg");
  drv_lcd_write(2, 1, lcd_string, strlen(lcd_string));
 
  printf ("LCD TEST APPLICATION - message printed on LCD module:''%s'' \n", lcd_string);
*/

  strcpy (lcd_string, "A VIDA EH SURF...");
  drv_lcd_write(2, 3, lcd_string, strlen(lcd_string));
  strcpy (lcd_string, "O RESTO EH ONDA!  ");
  drv_lcd_write(3, 3, lcd_string, strlen(lcd_string));

  drv_lcd_close();
}
