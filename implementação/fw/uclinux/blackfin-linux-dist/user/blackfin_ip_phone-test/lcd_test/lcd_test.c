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
  printf ("############################################\n");
  printf ("  Blackfin IP Phone - LCD Test Application\n");
  printf ("############################################\n");

  drv_lcd_open();
  drv_lcd_init();

/* TEST */
  char lcd_string[20];

  drv_lcd_display_on();
  drv_lcd_clear_screen();
  drv_lcd_contrast(28);
  drv_lcd_brightness(4);

  strcpy (lcd_string, "                    ");
  drv_lcd_write(1, 1, lcd_string, sizeof(lcd_string));
  strcpy (lcd_string, " A VIDA EH SURF...  ");
  drv_lcd_write(2, 1, lcd_string, sizeof(lcd_string));
  strcpy (lcd_string, " O RESTO EH ONDA!   ");
  drv_lcd_write(3, 1, lcd_string, sizeof(lcd_string));
  strcpy (lcd_string, "                    ");
  drv_lcd_write(4, 1, lcd_string, sizeof(lcd_string));

  drv_lcd_close();
}
