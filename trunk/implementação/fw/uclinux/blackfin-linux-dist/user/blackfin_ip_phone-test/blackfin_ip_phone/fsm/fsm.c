/*
 * Project: BLACKFIN IP PHONE
 * 
 * fsm.c - Finite state machine.
 */

#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "queue.h"
#include "../drivers/lcd/lcd.h"
#include "ipphone.h"

extern LinphoneCore ipphone_core;
extern int screen_option_qty[MAX_HOR_SCREENS];
extern hor_scroll_screen_fields_t screen[MAX_HOR_SCREENS];
extern main_queue_t event_queue;

void fsm_init(fsm_t *fsm)
{
	fsm->state = FSM_ST_IDLE;
	fsm->function[FSM_ST_IDLE]						= fsm_st_idle;
	fsm->function[FSM_ST_MENU] 						= fsm_st_menu;
	fsm->function[FSM_ST_MENU_CONTACTS]		=	fsm_st_menu_contacts;
	fsm->function[FSM_ST_CONTACT_ADD]			= fsm_st_contact_add;
	fsm->function[FSM_ST_MENU_CALL_LOGS]	= fsm_st_menu_call_logs;
	fsm->function[FSM_ST_MENU_SETTINGS]		= fsm_st_menu_settings;
	fsm->function[FSM_ST_DIALING]					= fsm_st_dialing;

	fsm_st_idle(FSM_EVNT_NULL);
}

fsm_state_t fsm_st_idle(fsm_evnt_t evnt)
{
	char *name 			= "XUXA M.";
	char *identity	= "1001";
	char *time			= "17:05";
	char *day				= "SUN, 10/05/2012";
	char *left			= "MENU";
	char *right			= "CONTACTS";

	lcd_screen_idle(name, identity, time, day, left, right);

	switch (evnt)
	{
		case FSM_EVNT_GPBUTTON_LEFT:
			return FSM_ST_MENU;
		case FSM_EVNT_GPBUTTON_RIGHT:
			return FSM_ST_CONTACT_ADD;
		case FSM_EVNT_NULL:
			return FSM_ST_IDLE;
		default:
			if ((evnt >= FSM_EVNT_KEYPAD_1) && (evnt <= FSM_EVNT_KEYPAD_SHARP))
			{
				queue_insert(&event_queue, evnt);
				return FSM_ST_DIALING;
			}
			else
				fsm_error(FSM_ST_IDLE);
	}
}

fsm_state_t fsm_st_dialing(fsm_evnt_t evnt)
{
	extern char *dialing_edit_fields[DIALING_EDIT_SIZE];
	static alphanumeric_buffer buffer[BUFFER_SIZE(DIALING_EDIT_SIZE)];
	static edit_screen dialing_screen;
	char *url;
	
	drv_lcd_cursor(LCD_TOGGLE_ON);
	edit_screen_init(&dialing_screen, buffer, DIALING_EDIT_SIZE, 20, dialing_edit_fields);	

  switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_LEFT:
			if (buffer[0].buffer[0] != '\0')
			{
				if (buffer[1].buffer[0] != '\0')
					snprintf(url, 46, "sip:%s@%s", buffer[0].buffer, buffer[1].buffer);
				else
					url = buffer[0].buffer;
				
				ipphone_call(&ipphone_core, url);
				//return FSM_ST_OUTGOING_CALL;
			}
			break;
    case FSM_EVNT_GPBUTTON_RIGHT:
			edit_screen_uninit(&dialing_screen);
			drv_lcd_cursor(LCD_TOGGLE_OFF);
      return FSM_ST_IDLE;
		case FSM_EVNT_NAVSWITCH_LEFT:
			edit_screen_move_cursor(&dialing_screen, LEFT);
			break;
		case FSM_EVNT_NAVSWITCH_RIGHT:
			edit_screen_move_cursor(&dialing_screen, RIGHT);
			break;
		case FSM_EVNT_NAVSWITCH_UP:
			edit_screen_move_cursor(&dialing_screen, UP);
			break;
		case FSM_EVNT_NAVSWITCH_DOWN:
			edit_screen_move_cursor(&dialing_screen, DOWN);
			break;
		case FSM_EVNT_NAVSWITCH_SELECT:
			edit_screen_delete(&dialing_screen);
			break;
		case FSM_EVNT_KEYPAD_SHARP:
			edit_screen_text_transform(&dialing_screen);
			break;
    case FSM_EVNT_NULL:
      return FSM_ST_DIALING;
    default:
      if ((evnt >= FSM_EVNT_KEYPAD_1) && (evnt < FSM_EVNT_KEYPAD_SHARP))
			{
				edit_screen_add(&dialing_screen, evnt);        
			}
      else
        fsm_error(FSM_ST_IDLE);
  }

	print_edit_screen(&dialing_screen);
	return FSM_ST_DIALING;
}

fsm_state_t fsm_st_menu(fsm_evnt_t evnt)
{
	static int option_index = 0;

  switch (evnt)
  {
		case FSM_EVNT_NAVSWITCH_LEFT:
			option_index = option_index ? (option_index - 1) : (screen_option_qty[SCREEN_MENU] - 1);
			break;
		case FSM_EVNT_NAVSWITCH_RIGHT:
			option_index = (option_index == (screen_option_qty[SCREEN_MENU] - 1)) ? 0 : (option_index + 1);
			break;
		case FSM_EVNT_GPBUTTON_LEFT:
			return screen[SCREEN_MENU].options_map[option_index];	
		case FSM_EVNT_GPBUTTON_RIGHT:
			option_index = 0;
			return FSM_ST_IDLE;
			break;
    case FSM_EVNT_NULL:
      break;
    default:
      fsm_error(FSM_ST_MENU);
  }
	
	lcd_screen_hor_scroll(SCREEN_MENU, option_index);

	return FSM_ST_MENU;
}

fsm_state_t fsm_st_menu_contacts(fsm_evnt_t evnt)
{
  static int option_index = 0;

  switch (evnt)
  {
    case FSM_EVNT_NAVSWITCH_LEFT:
      option_index = option_index ? (option_index - 1) : (screen_option_qty[SCREEN_CONTACTS] - 1);
      break;
    case FSM_EVNT_NAVSWITCH_RIGHT:
      option_index = (option_index == (screen_option_qty[SCREEN_CONTACTS] - 1)) ? 0 : (option_index + 1);
      break;
    case FSM_EVNT_GPBUTTON_RIGHT:
      option_index = 0;
      return FSM_ST_MENU;
		case FSM_EVNT_GPBUTTON_LEFT:
			return screen[SCREEN_CONTACTS].options_map[option_index];
    case FSM_EVNT_NULL:
      break;
    default:
      fsm_error(FSM_ST_MENU_CONTACTS);
  }

  lcd_screen_hor_scroll(SCREEN_CONTACTS, option_index);

  return FSM_ST_MENU_CONTACTS;
}

fsm_state_t fsm_st_contact_add(fsm_evnt_t evnt)
{
  extern char *contacts_edit_fields[CONTACTS_EDIT_SIZE];
	static alphanumeric_buffer buffer[BUFFER_SIZE(CONTACTS_EDIT_SIZE)];
	static edit_screen contact_screen;
	char url[70];

	drv_lcd_cursor(LCD_TOGGLE_ON);
	edit_screen_init(&contact_screen, buffer, CONTACTS_EDIT_SIZE, 20, contacts_edit_fields);

  switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			edit_screen_uninit(&contact_screen);
      drv_lcd_cursor(LCD_TOGGLE_OFF);
      return FSM_ST_MENU_CONTACTS;
    case FSM_EVNT_GPBUTTON_LEFT:
      if ((buffer[0].buffer[0] != '\0') && (buffer[1].buffer[0] != '\0') && (buffer[2].buffer[0] != '\0'))
      {
/*
				drv_lcd_cursor(FALSE);
				lcd_screen_save();
				snprintf(url, 70, "%s <sip:%s@%s>", buffer[0].buffer, buffer[1].buffer, buffer[2].buffer);
				printf("XXXXXXXXXX: %d", ipphone_add_friend(&ipphone_core, url));
				edit_screen_uninit(&contact_screen);
        return FSM_ST_MENU_CONTACTS;
*/
      }
			break;
		case FSM_EVNT_NAVSWITCH_LEFT:
			edit_screen_move_cursor(&contact_screen, LEFT);
			break;
		case FSM_EVNT_NAVSWITCH_RIGHT:
			edit_screen_move_cursor(&contact_screen, RIGHT);
			break;
		case FSM_EVNT_NAVSWITCH_UP:
			edit_screen_move_cursor(&contact_screen, UP);
			break;
		case FSM_EVNT_NAVSWITCH_DOWN:
			edit_screen_move_cursor(&contact_screen, DOWN);
			break;
		case FSM_EVNT_NAVSWITCH_SELECT:
			edit_screen_delete(&contact_screen);
			break;
		case FSM_EVNT_KEYPAD_SHARP:
			edit_screen_text_transform(&contact_screen);
			break;
    case FSM_EVNT_NULL:
      break;
    default:
      if ((evnt >= FSM_EVNT_KEYPAD_1) && (evnt < FSM_EVNT_KEYPAD_SHARP))
			{
				edit_screen_add(&contact_screen, evnt);        
			}
      else
        fsm_error(FSM_ST_CONTACT_ADD);
  }

	print_edit_screen(&contact_screen);
	return FSM_ST_CONTACT_ADD;
}

fsm_state_t fsm_st_menu_call_logs(fsm_evnt_t evnt)
{
  static int option_index = 0;

  switch (evnt)
  {
    case FSM_EVNT_NAVSWITCH_LEFT:
      option_index = option_index ? (option_index - 1) : (screen_option_qty[SCREEN_CALL_LOGS] - 1);
      break;
    case FSM_EVNT_NAVSWITCH_RIGHT:
      option_index = (option_index == (screen_option_qty[SCREEN_CALL_LOGS] - 1)) ? 0 : (option_index + 1);
      break;
    case FSM_EVNT_GPBUTTON_RIGHT:
      option_index = 0;
      return FSM_ST_MENU;
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      fsm_error(FSM_ST_MENU_CALL_LOGS);
  }

  lcd_screen_hor_scroll(SCREEN_CALL_LOGS, option_index);

  return FSM_ST_MENU_CALL_LOGS;
}

fsm_state_t fsm_st_menu_settings(fsm_evnt_t evnt)
{
  static int option_index = 0;

  switch (evnt)
  {
    case FSM_EVNT_NAVSWITCH_LEFT:
      option_index = option_index ? (option_index - 1) : (screen_option_qty[SCREEN_SETTINGS] - 1);
      break;
    case FSM_EVNT_NAVSWITCH_RIGHT:
      option_index = (option_index == (screen_option_qty[SCREEN_SETTINGS] - 1)) ? 0 : (option_index + 1);
      break;
    case FSM_EVNT_GPBUTTON_RIGHT:
      option_index = 0;
      return FSM_ST_MENU;
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      fsm_error(FSM_ST_MENU_SETTINGS);
  }

  lcd_screen_hor_scroll(SCREEN_SETTINGS, option_index);

  return FSM_ST_MENU_SETTINGS;
}

void fsm_error(fsm_state_t state)
{
	printf("ERROR!\n");
	printf("State: %d\n", state);
	exit(1);
}
