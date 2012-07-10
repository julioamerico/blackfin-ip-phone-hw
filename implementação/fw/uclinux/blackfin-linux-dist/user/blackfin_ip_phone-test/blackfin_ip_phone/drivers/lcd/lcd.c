#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

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
 *	PRE-DEFINED SCREENS
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

void lcd_screen_save(void)
{
	drv_lcd_clear_screen();
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, "SAVING ...");
	usleep(800000);
}

void lcd_screen_empty_list(void)
{
	drv_lcd_clear_screen();
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, "EMPTY LIST");
	usleep(800000);
}

void lcd_screen_full_list(void)
{
  drv_lcd_clear_screen();
  lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, "LIST IS FULL");
  usleep(800000);
}

/*
 *	HORIZONTAL SCROLL SCREENS
 */

int screen_option_qty[MAX_HOR_SCREENS] =
{
  [SCREEN_MENU] 			= 3,
  [SCREEN_CONTACTS] 	= 4,
  [SCREEN_CALL_LOGS] 	= 3,
  [SCREEN_SETTINGS]		= 3,
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

static char *screen_contacts_options[4] =
{
	"LIST",
	"EDIT",
	"ADD",
	"DELETE",
};

static fsm_state_t screen_contacts_options_map[4] =
{
	FSM_ST_CONTACTS_LIST,
	FSM_ST_CONTACTS_EDIT,
	FSM_ST_CONTACT_ADD,
	FSM_ST_CONTACT_DELETE,
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

static char *screen_settings_options[3] =
{
	"ACCOUNT",
	"NETWORK",
	"DATE & TIME",
};

static fsm_state_t screen_settings_options_map[3] =
{
  FSM_ST_SETTINGS_ACCOUNT,
  FSM_ST_SETTINGS_NETWORK,
  FSM_ST_SETTINGS_DATE_TIME,
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
static void slicer_init(slicer *sl, int size){
	sl->offset = 0;
	sl->size = size;
	sl->cursor = -1;
}
static void slicer_move_cursor(alphanumeric_buffer *strc_buffer, Position pos){
	switch(pos){
		case LEFT:
			strc_buffer->slice.cursor--;
			break;
		case RIGHT:
			strc_buffer->slice.cursor++;
			break;		
	}
	if(strc_buffer->slice.cursor < -1){
		strc_buffer->slice.cursor = -1;
	}
	else{ 
		if(strc_buffer->slice.cursor > strc_buffer->slice.size - 1)
			strc_buffer->slice.cursor = strc_buffer->slice.size - 1;
	}
	if(strc_buffer->slice.cursor == -1 && strc_buffer->slice.offset > 0){
		strc_buffer->slice.offset--;
		strc_buffer->slice.cursor++;
	}
	if(strc_buffer->slice.cursor == strc_buffer->slice.size - 1 && strc_buffer->last_element_position > strc_buffer->slice.size + strc_buffer->slice.offset - 1){
		strc_buffer->slice.offset++;
		strc_buffer->slice.cursor--;
	}	
}
static void slicer_update(alphanumeric_buffer *strc_buffer){
	if(strc_buffer->last_element_position < strc_buffer->size_buffer - 1 && strc_buffer->last_element_position > strc_buffer->slice.size - 1){
		if(strc_buffer->slice.cursor >= strc_buffer->slice.size/2){
			strc_buffer->slice.offset++;
			return;	
		}
	}
	slicer_move_cursor(strc_buffer, RIGHT);
}
static slicer_delete(alphanumeric_buffer *strc_buffer){
	if(strc_buffer->last_element_position < strc_buffer->slice.size + strc_buffer->slice.offset){
		if(strc_buffer->slice.offset > 0){
			strc_buffer->slice.offset--;
			return;
		}
	}
	slicer_move_cursor(strc_buffer, LEFT);
}
static void print_slice(alphanumeric_buffer *strc_buffer, int begin){
	int length = strc_buffer->slice.size;
	char wData[1];
	wData[0] = begin;

	if(strc_buffer->last_element_position < strc_buffer->slice.size){
		length = strlen(strc_buffer->buffer);	
	}
	drv_lcd_send_command(LCD_CMD_SET_CURSOR, wData, 1);	
	drv_lcd_send_data(strc_buffer->buffer + strc_buffer->slice.offset, length);
}
static void print_cursor(alphanumeric_buffer *strc_buffer, int begin){
	char wData[1];
	wData[0] = begin + 1 + strc_buffer->slice.cursor;
	drv_lcd_send_command(LCD_CMD_SET_CURSOR, wData, 1);
}

static void print_text_type(alphanumeric_buffer *strc_buffer){
	char *text_type;
	switch(strc_buffer->text.type){
		case UPPERCASE:
			text_type = "ABC";
			break;
		case LOWERCASE:
			text_type = "abc";
			break;			
		case CAPITALIZE:
			text_type = "Abc"; 
			break;
		case ONLYNUMBERS:
			text_type = "123";
			break;
	}
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 1, text_type);
}
char keypad_1[3] = {'1', '.', '@'};
char keypad_2[4] = {'2', 'a', 'b', 'c'};
char keypad_3[4] = {'3', 'd', 'e', 'f'};
char keypad_4[4] = {'4', 'g', 'h', 'i'};
char keypad_5[4] = {'5', 'j', 'k', 'l'};
char keypad_6[4] = {'6', 'm', 'n', 'o'};
char keypad_7[5] = {'7', 'p', 'q', 'r', 's'};
char keypad_8[4] = {'8', 't', 'u', 'v'};
char keypad_9[5] = {'9', 'w', 'x', 'y', 'z'};
char keypad_asterisk[1] = {'*'};
char keypad_0[2] = {'0', ' '};
char keypad_sharp[5] = {'#', '!', '?', ',', '.'};

static vertical_scroll key_map[] = {
	{
		.key = keypad_1,
		.size = 3,	
	},
	{
		.key = keypad_2,
		.size = 4,	
	},
	{
		.key = keypad_3,
		.size = 4,	
	},
	{
		.key = keypad_4,
		.size = 4,	
	},
	{
		.key = keypad_5,
		.size = 4,	
	},
	{
		.key = keypad_6,
		.size = 4,	
	},
	{
		.key = keypad_7,
		.size = 5,	
	},
	{
		.key = keypad_8,
		.size = 4,	
	},
	{
		.key = keypad_9,
		.size = 5,	
	},
	{
		.key = keypad_asterisk,
		.size = 1,	
	},
	{
		.key = keypad_0,
		.size = 2,	
	},
	{
		.key = keypad_sharp,
		.size = 5,	
	},
};

static int alphanumeric_buffer_init(alphanumeric_buffer *strc_buffer, int size_buffer, int size_slice){
	if(strc_buffer->is_init)
		return 0;
	strc_buffer->is_init = TRUE;
	strc_buffer->size_buffer = size_buffer;
	strc_buffer->map = key_map;
	strc_buffer->cursor = -1;
	strc_buffer->index = 1;
	strc_buffer->previous_event = -1;
	strc_buffer->last_element_position = -1;
	strc_buffer->previous_time.tv_usec = 0;
	strc_buffer->previous_time.tv_sec = 0;
	strc_buffer->text.type = CAPITALIZE;
	slicer_init(&strc_buffer->slice, size_slice);
	if((strc_buffer->buffer = (char *)malloc(size_buffer*sizeof(char))) == NULL)
	{
		return 1;
	}
	strc_buffer->buffer[0] = '\0';
	return 2;
}
static void alphanumeric_buffer_uninit(alphanumeric_buffer *strc_buffer){
	strc_buffer->is_init = FALSE;
	ipphone_free(strc_buffer->buffer);	
	return;
}

static void text_transform_update(alphanumeric_buffer *strc_buffer){
	if(strc_buffer->cursor < 0){
		strc_buffer->text.control = 0;	
	}
	if(strc_buffer->text.type == CAPITALIZE && strc_buffer->text.control++ == 1){
		strc_buffer->text.type = LOWERCASE;
		strc_buffer->text.control = 0;
	}
}
static void alphanumeric_buffer_add(alphanumeric_buffer *strc_buffer, fsm_evnt_t event){
	struct timeval actual_time;
	int dt, index, adjust;

	gettimeofday(&actual_time, NULL);
	dt = actual_time.tv_sec - strc_buffer->previous_time.tv_sec;
	if(actual_time.tv_usec - strc_buffer->previous_time.tv_usec > 0)
		dt++; 

	if(dt > 1 || event != strc_buffer->previous_event || strc_buffer->text.type == ONLYNUMBERS){
		strc_buffer->index = 1;

		text_transform_update(strc_buffer);
		if(strc_buffer->last_element_position < strc_buffer->size_buffer - 1){
			int i;
			for(i = strc_buffer->last_element_position + 1; i > strc_buffer->cursor; i--){
				strc_buffer->buffer[i + 1] = strc_buffer->buffer[i];	
			}			
			strc_buffer->last_element_position++;
		}
		if(strc_buffer->cursor < strc_buffer->size_buffer - 1){
			strc_buffer->cursor++;
		}
		slicer_update(strc_buffer);
	}
	if(strc_buffer->text.type == ONLYNUMBERS)
		index = 0;
	else
		index = strc_buffer->index++;
	strc_buffer->buffer[strc_buffer->cursor] = strc_buffer->map[event].key[index%(strc_buffer->map[event].size)];

	if(strc_buffer->buffer[strc_buffer->cursor] > 0x60 && strc_buffer->buffer[strc_buffer->cursor] < 0x7B){

		switch(strc_buffer->text.type){
			case UPPERCASE:
				adjust = 0x20;
				break;
			case LOWERCASE:
				adjust = 0x00;
				break;
			case CAPITALIZE:
				adjust = 0x20; 
				break;
			default:
				adjust = 0x00;
		}		
		strc_buffer->buffer[strc_buffer->cursor] -= adjust;
	} 	 
	strc_buffer->previous_event = event;
	strc_buffer->previous_time.tv_sec = actual_time.tv_sec;
	strc_buffer->previous_time.tv_usec = actual_time.tv_usec;
	return;
}
static void alphanumeric_buffer_delete(alphanumeric_buffer *strc_buffer){
	int i;
	if(strc_buffer->cursor < 0){		
		return;
	}
	slicer_delete(strc_buffer);
	for(i = strc_buffer->cursor; i < strc_buffer->last_element_position + 1; i++){
		strc_buffer->buffer[i] = strc_buffer->buffer[i + 1];	
	}
	strc_buffer->last_element_position--;
	strc_buffer->cursor--;
	strc_buffer->text.control = 0;
	return;
}
static void move_cursor(alphanumeric_buffer *strc_buffer, int pos){
	switch(pos){
		case LEFT:
			if(strc_buffer->cursor > -1){
				strc_buffer->cursor--;
				slicer_move_cursor(strc_buffer, LEFT);
			}
			else{
				strc_buffer->text.type = CAPITALIZE;
				strc_buffer->text.control = 0;
			}
			break;
		case RIGHT:
			if(strc_buffer->cursor < strc_buffer->last_element_position){
				strc_buffer->cursor++;
				slicer_move_cursor(strc_buffer, RIGHT);	
			}
			break;
	}
}
static void change_text_transform(alphanumeric_buffer *strc_buffer){
	strc_buffer->text.type = (++strc_buffer->text.type)%4;
}

static void print_buffer(alphanumeric_buffer *strc_buffer){
	char wData[1], begin = LCD_CURSOR_POS_R3_C1;
	wData[0] = begin;
	drv_lcd_send_command(LCD_CMD_SET_CURSOR, wData, 1);	
	drv_lcd_send_data(strc_buffer->buffer, strlen(strc_buffer->buffer));
	wData[0] = begin + 1 + strc_buffer->cursor;
	drv_lcd_send_command(LCD_CMD_SET_CURSOR, wData, 1);
	return;
}

char *dialing_edit_fields[DIALING_EDIT_SIZE] =
{
	"DIAL",
	"CALL",
	"EXIT",
	"Username:",
	"Server:",
};

char *contacts_edit_fields[CONTACTS_EDIT_SIZE] =
{
	"CONTACTS",
	"SAVE",
	"BACK",
	"Name:",
	"Number:",
	"SIP Server:",
};

static void alphanumeric_buffer_set(alphanumeric_buffer *strc_buffer, char *str){
	int str_length = strlen(str) - 1;
	if(str_length > strc_buffer->size_buffer - 1)
		return;
	strcpy(strc_buffer->buffer, str);	
	strc_buffer->cursor = str_length;
	strc_buffer->last_element_position = str_length;
	strc_buffer->slice.offset = str_length - strc_buffer->slice.size + 1;
	if(strc_buffer->slice.offset < 0){
		strc_buffer->slice.offset = 0;
		strc_buffer->slice.cursor = str_length;
	}
	else
		strc_buffer->slice.cursor = strc_buffer->slice.size - 1;
}

void edit_screen_init_params(edit_screen *edit, alphanumeric_buffer *buffer, int size_vet_buffer, int size_buffer, char **edit_fields, void *data,void(*get_fields)(void*,char **,int)){
	int i;
	char *fields;
	if(buffer[0].is_init)
		return;
	edit_screen_init(edit, buffer, size_vet_buffer, size_buffer, edit_fields);
	for(i = 0; i < edit->size; i++){
		get_fields(data, &fields, i);
		alphanumeric_buffer_set(buffer + i,fields);
	}
	ipphone_free(fields);
	
}

void edit_screen_init(edit_screen *edit, alphanumeric_buffer *buffer, int size_vet_buffer, int size_buffer, char **edit_fields){
	int i, size_slice;
	edit->vet_buffer = buffer;
	edit->size = size_vet_buffer - 3;

	for(i = 0; i < edit->size; i++){
		size_slice = LCD_MAX_COL - strlen(edit_fields[i + 3]) - 1;
		if(!alphanumeric_buffer_init(edit->vet_buffer + i, size_buffer, size_slice))
			return;
	}
	edit->row = 0;
	edit->offset = 0;
	edit->fields = edit_fields;
}
void edit_screen_uninit(edit_screen *edit){
	int i;
	for(i = 0; i < edit->size; i++){
		alphanumeric_buffer_uninit(edit->vet_buffer + i);
	}
}
void print_edit_screen(edit_screen *edit){
	int i, lcd_cursor_pos[] = {LCD_CURSOR_POS_R2_C1, LCD_CURSOR_POS_R3_C1};	
	
	drv_lcd_clear_screen();
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, edit->fields[0]);
	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 4, edit->fields[1]);
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 4, edit->fields[2]);

	for(i = 0; i < 2; i++){
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, i + 2, edit->fields[edit->offset + i + 3]);
		print_slice(edit->vet_buffer + edit->offset + i, lcd_cursor_pos[i] + strlen(edit->fields[edit->offset + i + 3]));
	}
	print_text_type(edit->vet_buffer + edit->offset + edit->row);
	print_cursor(edit->vet_buffer + edit->offset + edit->row, lcd_cursor_pos[edit->row] + strlen(edit->fields[edit->offset + edit->row + 3]));
}
void edit_screen_move_cursor(edit_screen *edit, Position pos){
	switch(pos){
		case UP:
			if(edit->row == 0 && edit->offset > 0)
				edit->offset--;
			edit->row = 0;
			break;
		case DOWN:
			if(edit->row == 1 && edit->offset < edit->size - 2){
				edit->offset++;
				edit->row = 0;
			}
			edit->row = 1;			
			break;
		default:
			move_cursor(edit->vet_buffer + edit->offset + edit->row, pos);
	}
}
void edit_screen_delete(edit_screen *edit){
	alphanumeric_buffer_delete(edit->vet_buffer + edit->offset + edit->row);	
}
void edit_screen_text_transform(edit_screen *edit){
	int i;	
	for(i = 0; i < edit->size; i++){
		change_text_transform(edit->vet_buffer + edit->offset + i);
	}	
}
void edit_screen_add(edit_screen *edit, fsm_evnt_t event){	
	alphanumeric_buffer_add(edit->vet_buffer + edit->offset + edit->row, event);
}
