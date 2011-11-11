/*
 * Project: BLACKFIN IP PHONE
 * 
 * drv_lcd.h - LCD driver
 *
 * Device P/N: NHD-0420D3Z-FL-GBW 
 * Device Manufacuter: Newhaven Display
 * Device Description:
 * 	- Serial LCD character module
 *      - 4 lines x 20 characters
 *	- PIC16F690 serial communication
 */

/*
 * Optional TODO's:
 * command - move cursor right one space
 * command - backspace
 * command - load custom character
 * command - shift display to the left
 * command - shift display to the right
*/

#ifndef DRV_LCD_H
#define DRV_LCD_H

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

#define LCD_MAX_COL				0x14
#define LCD_MAX_ROW				0x04

#define LCD_TOGGLE_ON				0x01
#define LCD_TOGGLE_OFF				0x00

#define LCD_CURSOR_POS_R1_C1			0x00
#define LCD_CURSOR_POS_R2_C1			0x40
#define LCD_CURSOR_POS_R3_C1			0x14
#define LCD_CURSOR_POS_R4_C1			0x54

#define LCD_CMD_PREFIX                          0xFE

#define LCD_CMD_DISPLAY_ON                      0x41
#define LCD_CMD_DISPLAY_OFF                     0x42
#define LCD_CMD_SET_CURSOR                      0x45
#define LCD_CMD_CURSOR_HOME                     0x46
#define LCD_CMD_UNDERLINE_CURSOR_ON             0x47
#define LCD_CMD_UNDERLINE_CURSOR_OFF            0x48
#define LCD_CMD_MOVE_CURSOR_LEFT_ONE_PLACE      0x49
#define LCD_CMD_MOVE_CURSOR_RIGHT_ONE_PLACE     0x4A
#define LCD_CMD_BLINKING_CURSOR_ON              0x4B
#define LCD_CMD_BLINKING_CURSOR_OFF             0x4C
#define LCD_CMD_BACKSPACE                       0x4E
#define LCD_CMD_CLEAR_SCREEN                    0x51
#define LCD_CMD_SET_CONTRAST                    0x52
#define LCD_CMD_SET_BACKLIGHT_BRIGHTNESS        0x53
#define LCD_CMD_LOAD_CUSTOM_CHARACTER           0x54
#define LCD_CMD_MOVE_ONE_PLACE_TO_LEFT          0x55
#define LCD_CMD_MOVE_ONE_PLACE_TO_RIGHT         0x56
#define LCD_CMD_CHANGE_RS232_BAUD_RATE          0x61
#define LCD_CMD_CHANGE_I2C_ADDRESS              0x62
#define LCD_CMD_DISPLAY_FW_VERSION              0x70
#define LCD_CMD_DISPLAY_RS232_BAUD_RATE         0x71
#define LCD_CMD_DISPLAY_I2C_ADDRESS             0x72

#define LCD_I2C_SLAVE_ADDR                      0x50

static int twi_fd;

/* I2C / COMMUNICATION */

void drv_i2c_write_byte(const char data)
{
  i2c_smbus_write_byte(twi_fd, data);
}

void drv_lcd_send_data(const char *data, const unsigned int len)
{
  int i = 0;

  for (i = 0; i < len; i++)
    drv_i2c_write_byte(data[i]);
}

void drv_lcd_send_command(const char command, const char *data, const unsigned int len)
{
  drv_i2c_write_byte(LCD_CMD_PREFIX);
  drv_i2c_write_byte(command);
  drv_lcd_send_data(data,len);
}

/* FILE DESCRIPTOR */

int drv_lcd_open(void)
{
  twi_fd = open (BFIN_LCD_TWI_PORT, O_RDWR);
  if (twi_fd < 0) {
    printf ("ERROR: can't open LCD file descriptor\n");
    return -1;
  }
}

void drv_lcd_close(void)
{
  close (twi_fd);
}

/* LCD COMMANDS */

int drv_lcd_init(void)
{
  int addr = LCD_I2C_SLAVE_ADDR;
  int err = 0;

  addr >>= 1;
  err = ioctl(twi_fd, I2C_SLAVE, addr);
  if (err < 0) {
    printf ("ERROR: ioctl() error number = %d\n", err);
    return -1;
  }

  drv_lcd_display(LCD_TOGGLE_ON);
  drv_lcd_clear_screen();
  drv_lcd_brightness(8);
  drv_lcd_contrast(28);
}

void drv_lcd_write(const int row, const int col, const char *data, int len)
/* row = [1..4]  */
/* col = [1..20] */
{
    char wData[1];

    if (col < 1 || col > 20)
	printf ("ERROR: invalid LCD column number\n");        
    if (len > (LCD_MAX_COL - col + 1))
	printf ("ERROR: data length exceeds columns left in row %d \n", row);

    switch (row) {
    case 1:
        wData[0] = LCD_CURSOR_POS_R1_C1 + col - 1;
        break;
    case 2:
        wData[0] = LCD_CURSOR_POS_R2_C1 + col - 1;
        break;
    case 3:
        wData[0] = LCD_CURSOR_POS_R3_C1 + col - 1;
        break;
    case 4:
        wData[0] = LCD_CURSOR_POS_R4_C1 + col - 1;
        break;
    default:
	printf ("ERROR: invalid LCD row number\n");
    }

    drv_lcd_send_command(LCD_CMD_SET_CURSOR, wData, 1);
    drv_lcd_send_data(data, len);
}

void drv_lcd_brightness(int brightness)
/* brightness = [1..8], default = 1 */
{
  char wData[1];
  wData[0] = brightness;
  drv_lcd_send_command(LCD_CMD_SET_BACKLIGHT_BRIGHTNESS, wData, 1);
}

void drv_lcd_contrast(int contrast)
/* contrast = [1..50], default = 40 */
{
  char wData[1];
  wData[0] = contrast;
  drv_lcd_send_command(LCD_CMD_SET_CONTRAST, wData, 1);
}

void drv_lcd_display(int cmd)
{
  if (cmd)
    drv_lcd_send_command(LCD_CMD_DISPLAY_ON, NULL, 0);
  else
    drv_lcd_send_command(LCD_CMD_DISPLAY_OFF, NULL, 0);
}

void drv_lcd_clear_screen(void)
{
  drv_lcd_send_command(LCD_CMD_CLEAR_SCREEN, NULL, 0);
}

void drv_lcd_cursor(int cmd)
{
  if (cmd)
    drv_lcd_send_command(LCD_CMD_UNDERLINE_CURSOR_ON, NULL, 0);
  else
    drv_lcd_send_command(LCD_CMD_UNDERLINE_CURSOR_OFF, NULL, 0);
}

void drv_lcd_cursor_blink(int cmd)
{
  if (cmd)
    drv_lcd_send_command(LCD_CMD_BLINKING_CURSOR_ON, NULL, 0);
  else
    drv_lcd_send_command(LCD_CMD_BLINKING_CURSOR_OFF, NULL, 0);
}

void drv_lcd_display_i2c_address(void)
{
  drv_lcd_send_command(LCD_CMD_DISPLAY_I2C_ADDRESS, NULL, 0);
}

void drv_lcd_display_fw_version(void)
{
  drv_lcd_send_command(LCD_CMD_DISPLAY_FW_VERSION, NULL, 0);
}

#endif /* DRV_LCD_H */
