/*
 * Project: BLACKFIN IP PHONE
 * 
 * fsm.c - Finite state machine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
	//fsm->function[FSM_ST_CALL_STATUS]			= fsm_st_call_status;
	fsm->function[FSM_ST_CONTACTS_LIST]		= fsm_st_contacts_list;
	fsm->function[FSM_ST_CONTACTS_EDIT]		= fsm_st_contacts_edit;
	fsm->function[FSM_ST_CONTACT_ADD]			= fsm_st_contact_add;
	fsm->function[FSM_ST_CONTACT_DELETE]	= fsm_st_contact_delete;
	fsm->function[FSM_ST_MENU_CALL_LOGS]	= fsm_st_menu_call_logs;
	fsm->function[FSM_ST_MENU_SETTINGS]		= fsm_st_menu_settings;
	fsm->function[FSM_ST_DIALING]					= fsm_st_dialing;

	fsm_st_idle(FSM_EVNT_NULL);
}

fsm_state_t fsm_st_idle(fsm_evnt_t evnt)
{
	char *name 			= "GORDIM";
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
				edit_screen_add(&dialing_screen, evnt);        
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
      break;
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
      break;
  }

  lcd_screen_hor_scroll(SCREEN_CONTACTS, option_index);
  return FSM_ST_MENU_CONTACTS;
}
/*
fsm_state_t fsm_st_call_status(fsm_evnt_t evnt)
{
	static int aux = 0;
	char lcd_line[3][20];
	static time_t current_time;

	current_time = time(NULL);
	
	printf("\n\n\n\n\nCURRENT TIME!!!!! %d\n\n\n\n", current_time);	
	
	if (call_time < 60segundos)
	{
		snprintf(lcd_line[3], 20, "TIME: %ds");
	}
	else if ((call_time >= 60segundos) && (call_time < 1hora))
	{
		snprintf(lcd_line[3], 20, "TIME: %dmin %dsec");
	}
	else if ((call_time >= 1hora) && (call_time < 9horas))
	{
		snprintf(lcd_line[3], 20, "TIME: %dh %dmin %dsec");
	}	

	if (!aux)
	{
		snprintf(lcd_line[1], 20, "STATUS: %s" , XXXXXX);
		snprintf(lcd_line[2], 20, "CONTACT: %s", XXXXXX);
		aux = 1;
	}
}
*/
fsm_state_t fsm_st_contacts_list(fsm_evnt_t evnt)
{
	static SubList contacts_list;

	drv_lcd_clear_screen();
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "CONTACT LIST");
	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 	4, "CALL");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 	4, "BACK");	

	if (sublist_init(ipphone_core.friends, &contacts_list, 2)	== EMPTY)
	{
		lcd_screen_empty_list();
		return FSM_ST_MENU_CONTACTS;
	}

	switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			sublist_uninit(&contacts_list);
      return FSM_ST_MENU_CONTACTS;
    case FSM_EVNT_GPBUTTON_LEFT:
    	//sublist_friend_call(&ipphone_core, &contacts_list);
			//return FSM_ST_CALL_STATUS;
    case FSM_EVNT_NAVSWITCH_UP:
      sublist_update(ipphone_core.friends, &contacts_list, UP);
      break;
    case FSM_EVNT_NAVSWITCH_DOWN:
      sublist_update(ipphone_core.friends, &contacts_list, DOWN);
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      break;
  }

	print_sublist_contacts(&contacts_list);
  return FSM_ST_CONTACTS_LIST;
}

fsm_state_t fsm_st_contacts_edit(fsm_evnt_t evnt)
{
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
				drv_lcd_cursor(LCD_TOGGLE_OFF);
				lcd_screen_save();
				snprintf(url, 70, "%s <sip:%s@%s>", buffer[0].buffer, buffer[1].buffer, buffer[2].buffer);
				ipphone_add_friend(&ipphone_core, url);
				edit_screen_uninit(&contact_screen);
        return FSM_ST_MENU_CONTACTS;
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
				edit_screen_add(&contact_screen, evnt);        
  }

	print_edit_screen(&contact_screen);
	return FSM_ST_CONTACT_ADD;
}

fsm_state_t fsm_st_contact_delete(fsm_evnt_t evnt)
{
  static SubList contacts_del_list;

  drv_lcd_clear_screen();
  lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "CONTACT LIST");
  lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   4, "DELETE");
  lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,  4, "BACK");

  if (sublist_init(ipphone_core.friends, &contacts_del_list, 2) == EMPTY)
  {
    lcd_screen_empty_list();
    return FSM_ST_MENU_CONTACTS;
  }

  switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			sublist_uninit(&contacts_del_list);
      return FSM_ST_MENU_CONTACTS;
    case FSM_EVNT_GPBUTTON_LEFT:
			sublist_friend_delete(&ipphone_core, &contacts_del_list);
  		if (sublist_is_empty(&contacts_del_list))
  		{
    		lcd_screen_empty_list();
    		return FSM_ST_MENU_CONTACTS;
		  }
			break;
    case FSM_EVNT_NAVSWITCH_UP:
      sublist_update(ipphone_core.friends, &contacts_del_list, UP);
      break;
    case FSM_EVNT_NAVSWITCH_DOWN:
      sublist_update(ipphone_core.friends, &contacts_del_list, DOWN);
      break;
    case FSM_EVNT_NULL:
      break;
		default:
			break;
	}
	
	print_sublist_contacts(&contacts_del_list);
	return FSM_ST_CONTACT_DELETE;
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
      break;
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
      break;
  }

  lcd_screen_hor_scroll(SCREEN_SETTINGS, option_index);

  return FSM_ST_MENU_SETTINGS;
}
