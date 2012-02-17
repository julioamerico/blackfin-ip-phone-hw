/*
 * Project: BLACKFIN IP PHONE
 * 
 * lcd.h - LCD driver
*/

#ifndef BLACKFIN_IP_PHONE_LCD_H
#define BLACKFIN_IP_PHONE_LCD_H

#include "drv_lcd.h"
#include "../../fsm/fsm.h"

typedef enum {
	LCD_WRITE_LEFT_JUSTIFIED,
	LCD_WRITE_RIGHT_JUSTIFIED,
	LCD_WRITE_CENTER_JUSTIFIED,

	LCD_WRITE_NULL
} lcd_write_justified_t;

typedef enum {
	SCREEN_MENU,
	SCREEN_CONTACTS,
	SCREEN_CALL_LOGS,
	SCREEN_SETTINGS,
	SCREEN_NULL
}	hor_scroll_screen_t;

#define MAX_HOR_SCREENS SCREEN_NULL

typedef struct hor_scroll_screen_fields {
	char *screen_name;
	char *left;
	char *right;
	char **screen_options;
	fsm_state_t *options_map;
} hor_scroll_screen_fields_t;

void lcd_init(void);
int lcd_write_justified(lcd_write_justified_t lcd_op, int row, char str[]);
void lcd_screen_idle(char *name, char *identity, char *time, char *day, char *left, char *right);

/*void lcd_screen_hor_scroll(char field_screen[], char field_left[], char field_right[], int option_index);*/

#endif /* BLACKFIN_IP_PHONE_LCD_H */
