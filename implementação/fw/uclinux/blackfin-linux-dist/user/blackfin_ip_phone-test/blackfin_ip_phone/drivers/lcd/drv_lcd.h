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

#ifndef BLACKFIN_IP_PHONE_DRV_LCD_H
#define BLACKFIN_IP_PHONE_DRV_LCD_H

#define BFIN_LCD_TWI_PORT	"/dev/i2c-0"

#define LCD_MAX_COL						0x14
#define LCD_MAX_ROW						0x04

#define LCD_TOGGLE_ON					0x01
#define LCD_TOGGLE_OFF				0x00

#define LCD_CURSOR_POS_R1_C1	0x00
#define LCD_CURSOR_POS_R2_C1	0x40
#define LCD_CURSOR_POS_R3_C1	0x14
#define LCD_CURSOR_POS_R4_C1	0x54

#define LCD_CMD_PREFIX        0xFE

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

void drv_i2c_write_byte(const char data);
void drv_lcd_send_data(const char *data, const unsigned int len);
void drv_lcd_send_command(const char command, const char *data, const unsigned int len);

int drv_lcd_open(void);
void drv_lcd_close(void);

int drv_lcd_init(void);
void drv_lcd_write(const int row, const int col, const char *data, int len);
void drv_lcd_brightness(int brightness);
void drv_lcd_contrast(int contrast);
void drv_lcd_display(int cmd);
void drv_lcd_clear_screen(void);
void drv_lcd_cursor(int cmd);
void drv_lcd_cursor_blink(int cmd);
void drv_lcd_display_i2c_address(void);
void drv_lcd_display_fw_version(void);

#endif /* BLACKFIN_IP_PHONE_DRV_LCD_H */
