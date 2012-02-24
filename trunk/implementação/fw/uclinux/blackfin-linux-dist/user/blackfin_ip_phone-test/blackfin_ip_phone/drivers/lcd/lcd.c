#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcd.h"

/*
 *	LCD SETUP
 */

void lcd_init(void)
{
  drv_lcd_open();
  drv_lcd_init();

  drv_lcd_cursor(LCD_TOGGLE_OFF);
  drv_lcd_cursor_blink(LCD_TOGGLE_OFF);
}

/*
 *	LCD WRITE OPTIONS
 */

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

/*
 *	IDLE SCREEN
 */

void lcd_screen_idle(char *name, char *identity, char *time, char *day, char *left, char *right)
{
	drv_lcd_clear_screen();
  lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 	1, name);
  lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 	1, identity);
  lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, time);
  lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, day);
  lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,		4, left);
  lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 	4, right);
}

/*
 *	HORIZONTAL SCROLL SCREENS
 */

int screen_option_qty[MAX_HOR_SCREENS] =
{
  [SCREEN_MENU] 			= 3,
  [SCREEN_CONTACTS] 	= 3,
  [SCREEN_CALL_LOGS] 	= 3,
  [SCREEN_SETTINGS]		= 5,
};

static char *screen_menu_options[3] =
{
	"CONTACTS",
	"CALL LOGS",
	"SETTINGS",
};

static fsm_state_t screen_menu_options_map[3] =
{
  FSM_ST_MENU_CONTACTS,
  FSM_ST_MENU_CALL_LOGS,
  FSM_ST_MENU_SETTINGS,
};

static char *screen_contacts_options[3] =
{
	"LIST",
	"EDIT",
	"ADD",
};

static fsm_state_t screen_contacts_options_map[3] =
{
	FSM_ST_CONTACTS_LIST,
	FSM_ST_CONTACTS_EDIT,
	FSM_ST_CONTACT_FIELDS,
};

static char *screen_call_logs_options[3] =
{
  "MISSED CALLS",
  "RECEIVED CALLS",
  "OUTGOING CALLS",
};

static fsm_state_t screen_call_logs_options_map[3] =
{
	FSM_ST_CALL_LOGS_MISSED,
	FSM_ST_CALL_LOGS_RECEIVED,
	FSM_ST_CALL_LOGS_OUTGOING,
};

static char *screen_settings_options[5] =
{
	"FW VERSION",
	"FW UPDATE",
	"SIP SERVER TEST",
	"CLEAR CONTACT LIST",
	"FACTORY SETTINGS",
};

static fsm_state_t screen_settings_options_map[5] =
{
  FSM_ST_SETTINGS_FW_VERSION,
  FSM_ST_SETTINGS_FW_UPDATE,
  FSM_ST_SETTINGS_SIP_SERVER_TEST,
  FSM_ST_SETTINGS_CLEAR_CONTACT_LIST,
  FSM_ST_SETTINGS_FACTORY_SETTINGS,

};

hor_scroll_screen_fields_t screen[MAX_HOR_SCREENS] =
{
	{
		.screen_name		= "MENU",
		.left						= "SELECT",
		.right					= "EXIT",
		.screen_options	=	screen_menu_options,
		.options_map		= screen_menu_options_map,
	}, {
    .screen_name    = "CONTACTS",
    .left           = "SELECT",
    .right          = "BACK",
    .screen_options = screen_contacts_options,
		.options_map		= screen_contacts_options_map,
  }, {
    .screen_name    = "CALL LOGS",
    .left           = "SELECT",
    .right          = "BACK",
    .screen_options = screen_call_logs_options,
		.options_map		= screen_call_logs_options_map,
  }, {
    .screen_name    = "SETTINGS",
    .left           = "SELECT",
    .right          = "BACK",
    .screen_options = screen_settings_options,
		.options_map		= screen_settings_options_map,
  },
};

void lcd_screen_hor_scroll(hor_scroll_screen_t sc, int option_index)
{
	drv_lcd_clear_screen();
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, screen[sc].screen_name);
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, screen[sc].screen_options[option_index]);
	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   3, "<");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,  3, ">");
	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   4, screen[sc].left);
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,  4, screen[sc].right);
}
