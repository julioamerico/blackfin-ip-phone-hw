/*
 * Project: BLACKFIN IP PHONE
 * 
 * lcd.h - LCD driver
*/

#ifndef BLACKFIN_IP_PHONE_LCD_H
#define BLACKFIN_IP_PHONE_LCD_H

#include "drv_lcd.h"
#include "../../fsm/fsm.h"
#include "ipphone.h"

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
#define CONTACTS_EDIT_SIZE 6
#define DIALING_EDIT_SIZE	 5
#define BUFFER_SIZE(x) x - 3
 
typedef struct hor_scroll_screen_fields {
	char *screen_name;
	char *left;
	char *right;
	char **screen_options;
	fsm_state_t *options_map;
} hor_scroll_screen_fields_t;

typedef struct _vertical_scroll{
	char *key;
	int size;
}vertical_scroll;

typedef enum _text_transform_types{
	UPPERCASE,
	LOWERCASE,
	CAPITALIZE,
	ONLYNUMBERS,
}text_transform_types;

typedef struct{
	text_transform_types type;
	int control;
}text_transform;

typedef struct{
	int offset;
	int size;
	int cursor;
}slicer;

typedef struct _alphanumeric_buffer{
	char *buffer;
	int size_buffer;
	int cursor;
	int last_element_position;
	int index;
	fsm_evnt_t previous_event;
	vertical_scroll *map;
	bool_t is_init;
	struct timeval previous_time;
	slicer slice;
	text_transform text;
}alphanumeric_buffer;

typedef struct{
	int row;
	int offset;
	int size;
	const char **fields;	
	const alphanumeric_buffer *vet_buffer;
}edit_screen;

void lcd_init(void);
int lcd_write_justified(lcd_write_justified_t lcd_op, int row, char str[]);
void lcd_screen_idle(char *name, char *identity, char *time, char *day, char *left, char *right);
void lcd_screen_save(void);
void lcd_screen_empty_list(void);
void lcd_screen_hor_scroll(hor_scroll_screen_t sc, int option_index);

void edit_screen_init_params(edit_screen *, alphanumeric_buffer *, int, int, char **, void *, void(*get_fields)(void*,char **,int));
void edit_screen_init(edit_screen *edit, alphanumeric_buffer *buffer, int size_vet_buffer, int size_buffer, char **edit_fields);
void edit_screen_uninit(edit_screen *edit);
void print_edit_screen(edit_screen *edit);
void edit_screen_move_cursor(edit_screen *edit, Position pos);
void edit_screen_add(edit_screen *edit, fsm_evnt_t event);
void edit_screen_delete(edit_screen *edit);
void edit_screen_text_transform(edit_screen *edit);

#endif /* BLACKFIN_IP_PHONE_LCD_H */
