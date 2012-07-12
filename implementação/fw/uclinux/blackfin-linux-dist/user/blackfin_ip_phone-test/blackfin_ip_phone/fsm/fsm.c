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
extern const char *nw_settings;
LinphoneFriend *edit_lf;

static void enable_dhcp();
static bool_t is_valid_ip(edit_screen *edit);
static void apply_static_nw_settings(alphanumeric_buffer *buffer);


void fsm_init(fsm_t *fsm)
{
	fsm->state = FSM_ST_IDLE;

	fsm->function[FSM_ST_IDLE]									= fsm_st_idle;
	fsm->function[FSM_ST_MENU] 									= fsm_st_menu;
	fsm->function[FSM_ST_MENU_CONTACTS]					=	fsm_st_menu_contacts;
	fsm->function[FSM_ST_INCOMING_CALL]					= fsm_st_incoming_call;
	fsm->function[FSM_ST_RECENT_LOST_CALLS]			= fsm_st_recent_lost_calls;
	fsm->function[FSM_ST_CALL_STATUS]						= fsm_st_call_status;
	fsm->function[FSM_ST_CONTACTS_LIST]					= fsm_st_contacts_list;
	fsm->function[FSM_ST_CONTACTS_EDIT]					= fsm_st_contacts_edit;
	fsm->function[FSM_ST_CONTACTS_EDIT_FIELDS]	= fsm_st_contacts_edit_fields;
	fsm->function[FSM_ST_CONTACT_ADD]						= fsm_st_contact_add;
	fsm->function[FSM_ST_CONTACT_DELETE]				= fsm_st_contact_delete;
	fsm->function[FSM_ST_MENU_CALL_LOGS]				= fsm_st_menu_call_logs;
	fsm->function[FSM_ST_CALL_LOGS_MISSED]			= fsm_st_call_logs_missed;
	fsm->function[FSM_ST_CALL_LOGS_RECEIVED]		= fsm_st_call_logs_received;
	fsm->function[FSM_ST_CALL_LOGS_OUTGOING]		= fsm_st_call_logs_outgoing;
	fsm->function[FSM_ST_MENU_SETTINGS]					= fsm_st_menu_settings;
	fsm->function[FSM_ST_SETTINGS_ACCOUNT]			= fsm_st_settings_account;
	fsm->function[FSM_ST_SETTINGS_NETWORK]			= fsm_st_settings_network;
	fsm->function[FSM_ST_SETTINGS_DATE_TIME]		= fsm_st_settings_date_time;
	fsm->function[FSM_ST_NETWORK_STATIC]				= fsm_st_network_static;
	fsm->function[FSM_ST_DIALING]								= fsm_st_dialing;
	fsm->function[FSM_ST_EDIT_DATE]							= fsm_st_edit_date;
	fsm->function[FSM_ST_EDIT_TIME]							= fsm_st_edit_time;

	fsm_st_idle(FSM_EVNT_NULL);
}

fsm_state_t fsm_st_idle(fsm_evnt_t evnt)
{
	static int aux = 0, previous_min = 0;
	int current_min = 0;
	char time_val[6];
	char date [16];
  struct tm *tm_ptr;
  time_t t;
	
  t = time((time_t *)0);
  tm_ptr = localtime(&t);
	current_min = tm_ptr->tm_min;

  if ((current_min != previous_min) && (aux == 1))
  {
    strftime(time_val, 6, "%H:%M", tm_ptr);
    strftime(date, 16, "%a, %d/%m/%Y", tm_ptr);

    lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, time_val);
    lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, date);
  }

	previous_min = current_min;

	if (!aux)
	{
    strftime(time_val, sizeof(time_val), "%H:%M", tm_ptr);
    strftime(date, sizeof(date), "%a, %d/%m/%Y", tm_ptr);

		drv_lcd_clear_screen();
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   1, "XXXXX");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,  1, "XXXXX");
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, time_val);
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, date);
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   4, "MENU");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,  4, "CONTACTS");

		aux = 1;
	}
	
	switch (evnt)
	{
		case FSM_EVNT_GPBUTTON_LEFT:
			aux = 0;
			return FSM_ST_MENU;
		case FSM_EVNT_GPBUTTON_RIGHT:
			aux = 0;
			return FSM_ST_CONTACTS_LIST;
		case FSM_EVNT_CALL_IN_INVITE:
			aux = 0;
			return FSM_ST_INCOMING_CALL;
		case FSM_EVNT_NULL:
			break;
		default:
			if ((evnt >= FSM_EVNT_KEYPAD_1) && (evnt <= FSM_EVNT_KEYPAD_SHARP))
			{
				aux = 0;
				queue_insert(&event_queue, evnt);
				return FSM_ST_DIALING;
			}
	}

	queue_insert(&event_queue, FSM_EVNT_NULL);
	return FSM_ST_IDLE;
}

fsm_state_t fsm_st_dialing(fsm_evnt_t evnt)
{
	extern char *dialing_edit_fields[DIALING_EDIT_SIZE];
	static alphanumeric_buffer buffer[BUFFER_SIZE(DIALING_EDIT_SIZE)];
	static edit_screen dialing_screen;
	char url[46];
	
	drv_lcd_cursor(LCD_TOGGLE_ON);
	edit_screen_init(&dialing_screen, buffer, DIALING_EDIT_SIZE, 20, dialing_edit_fields);	

  switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_LEFT:
			if (buffer[0].buffer[0] != '\0')
			{
				if (buffer[1].buffer[0] != '\0')
					snprintf(url, 46, "sip:%s@%s", buffer[0].buffer, buffer[1].buffer);
				else{
					snprintf(url,46,"%s",buffer[0].buffer);
		
				}
	     
			  drv_lcd_cursor(LCD_TOGGLE_OFF);
  		  edit_screen_uninit(&dialing_screen);
				ipphone_call(&ipphone_core, url);
				return FSM_ST_CALL_STATUS;
			}
			break;
    case FSM_EVNT_GPBUTTON_RIGHT:
			drv_lcd_cursor(LCD_TOGGLE_OFF);
			edit_screen_uninit(&dialing_screen);
			ipphone_terminate_call(&ipphone_core);
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
      break;
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

fsm_state_t fsm_st_incoming_call(fsm_evnt_t evnt)
{
	static int aux = 0;
	char *string;

	if (!aux)
	{
		drv_lcd_clear_screen();
		ipphone_call_get_contacts(&ipphone_core, &string, 20);
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, string);
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 4, "ACCEPT");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 4, "REJECT");
		ipphone_free(string);
		aux = 1;
	}

  switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			ipphone_terminate_call(&ipphone_core);
			aux = 0;
      return FSM_ST_IDLE;
    case FSM_EVNT_GPBUTTON_LEFT:
			ipphone_accept_call(&ipphone_core);
			aux = 0;
			return FSM_ST_CALL_STATUS;
		case FSM_EVNT_CALL_END:
			aux = 0;
			return FSM_ST_RECENT_LOST_CALLS;
    case FSM_EVNT_NULL:
      break;
    default:
      break;
  }

  return FSM_ST_INCOMING_CALL;
}

fsm_state_t fsm_st_recent_lost_calls(fsm_evnt_t evnt)
{
  static int aux = 0;
	static int lost_calls = 0;
	char string[20];

  if (!aux)
  {
    drv_lcd_clear_screen();
		lost_calls = lost_calls + 1;
		if (lost_calls == 1)
			snprintf(string, 20, "1 LOST CALL", lost_calls);
		else
			snprintf(string, 20, "%d LOST CALLS", lost_calls);
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, string);
    lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 4, "LIST");
    lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 4, "EXIT");
    aux = 1;
  }

  switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
     	lost_calls = 0; 
      aux = 0;
      return FSM_ST_IDLE;
    case FSM_EVNT_GPBUTTON_LEFT:
      lost_calls = 0;
      aux = 0;
      return FSM_ST_CALL_LOGS_MISSED;
    case FSM_EVNT_CALL_IN_INVITE:
			aux = 0;
      return FSM_ST_INCOMING_CALL;
		case FSM_EVNT_NULL:
			break;
    default:
      break;
  }

  return FSM_ST_RECENT_LOST_CALLS;
}

fsm_state_t fsm_st_call_status(fsm_evnt_t evnt)
{
	static int aux1 = 0, aux2 = 0, aux3 = 0;
	int delta_time_sec = 0;
	static int delta_time_min = 0, delta_time_hour = 0;
	static int initial_time_sec;
	static int previous_time_sec = 0, previous_time_min = 0;
	char lcd_line[3][20];
	struct tm *current_time;
	time_t t;
	char *username;

  if (!aux1)
  {
		if (ipphone_call_get_contacts(&ipphone_core, &username, 11))
			username = "Unknown Contact";
		drv_lcd_clear_screen();
    snprintf(lcd_line[1], 20, "CONTACT: %s", username);
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,  2, lcd_line[1]);
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,  1, "STATUS: CONNECTING");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 4, "END CALL");
		ipphone_free(username);
		aux1 = 1;
  }
	
	if (aux2)
	{
		t = time(NULL);
		current_time = localtime(&t);
		if (!aux3)
		{
			initial_time_sec = current_time->tm_sec;
			aux3 = 1;
		}

	  if (current_time->tm_sec >= initial_time_sec)
      delta_time_sec = current_time->tm_sec - initial_time_sec;
    else
      delta_time_sec = current_time->tm_sec + 60 - initial_time_sec;
		
		if (previous_time_sec != delta_time_sec)
		{
			if ((previous_time_sec == 59) && (delta_time_sec == 0))
			{
				if (previous_time_min < 59)
					delta_time_min = delta_time_min + 1;
				else
				{
					delta_time_min = 0;
					delta_time_hour = delta_time_hour + 1;
					if (delta_time_hour == 10)
					{
						delta_time_sec = 59;
						delta_time_min = 59;
						delta_time_hour = 9;
						aux2 = 0;
					}
				}
			}	
		}

		if (previous_time_sec != delta_time_sec)
		{
			if ((delta_time_min == 0) && (delta_time_hour == 0))
				snprintf(lcd_line[2], 20, "TIME: %ds ", delta_time_sec);
			else if ((delta_time_min > 0) && (delta_time_hour == 0))
				snprintf(lcd_line[2], 20, "TIME: %dmin %dsec  ", delta_time_min, delta_time_sec);
			else if ((delta_time_hour > 0) && (delta_time_hour <= 9))
				snprintf(lcd_line[2], 20, "TIME: %dh %dmin %dsec", delta_time_hour, delta_time_min, delta_time_sec);

			lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 3, lcd_line[2]);
		}

		previous_time_sec = delta_time_sec;
    previous_time_min = delta_time_min;	
	}

	switch (evnt)
	{
		case FSM_EVNT_CALL_IN_CONNECTED:
		case FSM_EVNT_CALL_OUT_CONNECTED:
			snprintf(lcd_line[1], 20, "STATUS: %s" , "IN PROGRESS");
			lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 1, lcd_line[1]);
			aux2 = 1;
			break;
		case FSM_EVNT_GPBUTTON_RIGHT:
		case FSM_EVNT_CALL_END:
			snprintf(lcd_line[1], 20, "STATUS: %s" , "CALL ENDED ");
			lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 1, lcd_line[1]);
			ipphone_terminate_call(&ipphone_core);
			usleep(1000000);
			aux1 = 0;
			aux2 = 0;
			aux3 = 0;
			previous_time_sec = 0;
			previous_time_min = 0;
			delta_time_min = 0;
			delta_time_hour = 0;
			return FSM_ST_IDLE;
    case FSM_EVNT_LINPHONE_CALL_FAIL:
      usleep(2000000);
      aux1 = 0;
      aux2 = 0;
      aux3 = 0;
      previous_time_sec = 0;
      previous_time_min = 0;
      delta_time_min = 0;
      delta_time_hour = 0;
      return FSM_ST_IDLE;
		case FSM_EVNT_NULL:
			break;
    default:
      break;
	}

	queue_insert(&event_queue, FSM_EVNT_NULL);
	return FSM_ST_CALL_STATUS;
}

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
    	sublist_friend_call(&ipphone_core, &contacts_list);
			sublist_uninit(&contacts_list);
			return FSM_ST_CALL_STATUS;
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

fsm_state_t fsm_st_contacts_edit(fsm_evnt_t evnt){
	static SubList contacts_list;
	drv_lcd_clear_screen();
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "CONTACT LIST");
	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 	4, "SELECT");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 	4, "BACK");	

	if (sublist_init(ipphone_core.friends, &contacts_list, 2) == EMPTY){
		lcd_screen_empty_list();
		return FSM_ST_MENU_CONTACTS;
	}

	switch (evnt){
		case FSM_EVNT_GPBUTTON_RIGHT:
			sublist_uninit(&contacts_list);
    	return FSM_ST_MENU_CONTACTS;
    case FSM_EVNT_GPBUTTON_LEFT:
			edit_lf = contacts_list.vet[contacts_list.cursor]->data;
			sublist_uninit(&contacts_list);
			return FSM_ST_CONTACTS_EDIT_FIELDS;
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
	return FSM_ST_CONTACTS_EDIT;	
}

fsm_state_t fsm_st_contacts_edit_fields(fsm_evnt_t evnt){
	extern char *contacts_edit_fields[CONTACTS_EDIT_SIZE];
	static alphanumeric_buffer buffer[BUFFER_SIZE(CONTACTS_EDIT_SIZE)];
	static edit_screen contact_screen;
	char url[70];
	drv_lcd_cursor(LCD_TOGGLE_ON);
	edit_screen_init_params(&contact_screen, buffer, CONTACTS_EDIT_SIZE, 20, contacts_edit_fields, edit_lf, ipphone_get_friends_fields);

	switch (evnt){
		case FSM_EVNT_GPBUTTON_RIGHT:
			edit_screen_uninit(&contact_screen);
			drv_lcd_cursor(LCD_TOGGLE_OFF);
   		return FSM_ST_CONTACTS_EDIT;
   	case FSM_EVNT_GPBUTTON_LEFT:
			if ((buffer[0].buffer[0] != '\0') && (buffer[1].buffer[0] != '\0') && (buffer[2].buffer[0] != '\0')){
				drv_lcd_cursor(LCD_TOGGLE_OFF);
				lcd_screen_save();
				snprintf(url, 70, "%s <sip:%s@%s>", buffer[0].buffer, buffer[1].buffer, buffer[2].buffer);
				ipphone_edit_friend(&ipphone_core, edit_lf, url);
				edit_screen_uninit(&contact_screen);
			}
			return FSM_ST_CONTACTS_EDIT;
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
	return FSM_ST_CONTACTS_EDIT_FIELDS;	
}

fsm_state_t fsm_st_contact_add(fsm_evnt_t evnt)
{
  extern char *contacts_edit_fields[CONTACTS_EDIT_SIZE];
	static alphanumeric_buffer buffer[BUFFER_SIZE(CONTACTS_EDIT_SIZE)];
	static edit_screen contact_screen;
	char url[70];
	static int aux = 0;

	if (aux == 0)
	{
		if (is_friend_list_full(&ipphone_core))
		{
			drv_lcd_cursor(LCD_TOGGLE_OFF);
			lcd_screen_full_list();		
			return FSM_ST_MENU_CONTACTS;
		}
		aux = 1;
	}

	drv_lcd_cursor(LCD_TOGGLE_ON);
	edit_screen_init(&contact_screen, buffer, CONTACTS_EDIT_SIZE, 20, contacts_edit_fields);

  switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			edit_screen_uninit(&contact_screen);
      drv_lcd_cursor(LCD_TOGGLE_OFF);
			aux = 0;
      return FSM_ST_MENU_CONTACTS;
    case FSM_EVNT_GPBUTTON_LEFT:
      if ((buffer[0].buffer[0] != '\0') && (buffer[1].buffer[0] != '\0') && (buffer[2].buffer[0] != '\0'))
      {
				drv_lcd_cursor(LCD_TOGGLE_OFF);
				lcd_screen_save();
				snprintf(url, 70, "%s <sip:%s@%s>", buffer[0].buffer, buffer[1].buffer, buffer[2].buffer);
				ipphone_add_friend(&ipphone_core, url);
				edit_screen_uninit(&contact_screen);
				aux = 0;
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
	int option_index_copy;

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
		case FSM_EVNT_GPBUTTON_LEFT:
			option_index_copy = option_index;
			option_index = 0;
			return screen[SCREEN_CALL_LOGS].options_map[option_index_copy];
    case FSM_EVNT_NULL:
      break;
    default:
      break;
  }

  lcd_screen_hor_scroll(SCREEN_CALL_LOGS, option_index);

  return FSM_ST_MENU_CALL_LOGS;
}

fsm_state_t fsm_st_call_logs_missed(fsm_evnt_t evnt)
{
	static SubList call_logs_missed_list;
	static int aux = 0;

	if (!aux)
	{
		drv_lcd_clear_screen();
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "MISSED CALLS");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 	4, "CALL");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 	4, "BACK");
		aux = 1;
	}

	if (sublist_init(ipphone_core.m_calls, &call_logs_missed_list, 2)	== EMPTY)
	{
		lcd_screen_empty_list();
		return FSM_ST_MENU_CALL_LOGS;
	}

	switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			sublist_uninit(&call_logs_missed_list);
			aux = 0;
      return FSM_ST_MENU_CALL_LOGS;
    case FSM_EVNT_GPBUTTON_LEFT:
    	sublist_call_log_call(&ipphone_core, &call_logs_missed_list, ipphone_calllog_get_from);
			sublist_uninit(&call_logs_missed_list);
			aux = 0;
			return FSM_ST_CALL_STATUS;
    case FSM_EVNT_NAVSWITCH_UP:
      sublist_update(ipphone_core.m_calls, &call_logs_missed_list, UP);
      break;
    case FSM_EVNT_NAVSWITCH_DOWN:
      sublist_update(ipphone_core.m_calls, &call_logs_missed_list, DOWN);
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      break;
  }

	print_sublist_call_logs(&ipphone_core, &call_logs_missed_list, ipphone_calllog_get_from);
  return FSM_ST_CALL_LOGS_MISSED;
}

fsm_state_t fsm_st_call_logs_received(fsm_evnt_t evnt)
{
	static SubList call_logs_received_list;
	static int aux = 0;

	if (!aux)
	{
		drv_lcd_clear_screen();
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "RECEIVED CALLS");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 	4, "CALL");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 	4, "BACK");
		aux = 1;
	}

	if (sublist_init(ipphone_core.r_calls, &call_logs_received_list, 2)	== EMPTY)
	{
		lcd_screen_empty_list();
		return FSM_ST_MENU_CALL_LOGS;
	}

	switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			sublist_uninit(&call_logs_received_list);
			aux = 0;
      return FSM_ST_MENU_CALL_LOGS;
    case FSM_EVNT_GPBUTTON_LEFT:
    	sublist_call_log_call(&ipphone_core, &call_logs_received_list, ipphone_calllog_get_from);
			sublist_uninit(&call_logs_received_list);
			aux = 0;
			return FSM_ST_CALL_STATUS;
    case FSM_EVNT_NAVSWITCH_UP:
      sublist_update(ipphone_core.r_calls, &call_logs_received_list, UP);
      break;
    case FSM_EVNT_NAVSWITCH_DOWN:
      sublist_update(ipphone_core.r_calls, &call_logs_received_list, DOWN);
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      break;
  }

	print_sublist_call_logs(&ipphone_core, &call_logs_received_list, ipphone_calllog_get_from);
  return FSM_ST_CALL_LOGS_RECEIVED;
}

fsm_state_t fsm_st_call_logs_outgoing(fsm_evnt_t evnt)
{
	static SubList call_logs_outgoing_list;
	static int aux = 0;

	if (!aux)
	{
		drv_lcd_clear_screen();
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "OUTGOING CALLS");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 	4, "CALL");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 	4, "BACK");
		aux = 1;
	}

	if (sublist_init(ipphone_core.d_numbers, &call_logs_outgoing_list, 2)	== EMPTY)
	{
		lcd_screen_empty_list();
		return FSM_ST_MENU_CALL_LOGS;
	}

	switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			sublist_uninit(&call_logs_outgoing_list);
			aux = 0;
      return FSM_ST_MENU_CALL_LOGS;
    case FSM_EVNT_GPBUTTON_LEFT:
    	sublist_call_log_call(&ipphone_core, &call_logs_outgoing_list, ipphone_calllog_get_to);
			sublist_uninit(&call_logs_outgoing_list);
			aux = 0;
			return FSM_ST_CALL_STATUS;
    case FSM_EVNT_NAVSWITCH_UP:
      sublist_update(ipphone_core.d_numbers, &call_logs_outgoing_list, UP);
      break;
    case FSM_EVNT_NAVSWITCH_DOWN:
      sublist_update(ipphone_core.d_numbers, &call_logs_outgoing_list, DOWN);
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      break;
  }

	print_sublist_call_logs(&ipphone_core, &call_logs_outgoing_list, ipphone_calllog_get_to);
  return FSM_ST_CALL_LOGS_OUTGOING;
}

fsm_state_t fsm_st_menu_settings(fsm_evnt_t evnt)
{
  static int option_index = 0;
	int option_index_copy;

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
    case FSM_EVNT_GPBUTTON_LEFT:
      option_index_copy = option_index;
      option_index = 0;
      return screen[SCREEN_SETTINGS].options_map[option_index_copy];
    case FSM_EVNT_NULL:
      break;
    default:
      break;
  }

  lcd_screen_hor_scroll(SCREEN_SETTINGS, option_index);

  return FSM_ST_MENU_SETTINGS;
}

fsm_state_t fsm_st_settings_account(fsm_evnt_t evnt)
{
  extern char *account_edit_fields[ACCOUNT_EDIT_SIZE];
  extern const char *phone_info_path;
  static alphanumeric_buffer buffer[BUFFER_SIZE(ACCOUNT_EDIT_SIZE)];
  static edit_screen account_screen;
	LinphoneProxyConfig *edit_account;
	static int aux = 0;
	char write_to_file[50];
	char write_config_server[30];
	char write_config_identity[30];
	FILE *fd;
	int i;

	if (!aux)
	{
  	drv_lcd_cursor(LCD_TOGGLE_ON);
		edit_account = ipphone_core.default_proxy;
		edit_screen_init_params(&account_screen, buffer, ACCOUNT_EDIT_SIZE, 20, account_edit_fields, edit_account, ipphone_get_account_fields);
		aux = 1;
	}

	switch (evnt){
    case FSM_EVNT_GPBUTTON_RIGHT:
      edit_screen_uninit(&account_screen);
      drv_lcd_cursor(LCD_TOGGLE_OFF);
			aux = 0;
      return FSM_ST_MENU_SETTINGS;
    case FSM_EVNT_GPBUTTON_LEFT:
      if ((buffer[0].buffer[0] != '\0') && (buffer[1].buffer[0] != '\0') && (buffer[2].buffer[0] != '\0') && (buffer[3].buffer[0] != '\0'))
			{
				// saving password and phone name
				snprintf(write_to_file, 50, "phone_name = %s\npassword = %s", buffer[0].buffer, buffer[2].buffer);
			  if ((fd = fopen(phone_info_path, "w")) == NULL)
    			break;
				fprintf(fd, write_to_file);
				fclose(fd);
				linphone_core_clear_all_auth_info(&ipphone_core);

				// saving number and sip server
				snprintf(write_config_server, 30, "sip:%s@%s", buffer[3].buffer, buffer[3].buffer);
				snprintf(write_config_identity, 30, "sip:%s@%s", buffer[1].buffer, buffer[3].buffer);
				edit_account = ipphone_proxy_config_new();
  			ipphone_proxy_config_set_server_addr(edit_account, write_config_server);
  			ipphone_proxy_config_set_identity(edit_account, write_config_identity);
  			ipphone_proxy_config_enableregister(edit_account, TRUE);
  			ipphone_proxy_config_expires(edit_account, 1);
			  ipphone_add_proxy_config(&ipphone_core, edit_account);

        drv_lcd_cursor(LCD_TOGGLE_OFF);
        lcd_screen_save();
				edit_screen_uninit(&account_screen);
				aux = 0;
      	return FSM_ST_MENU_SETTINGS;
      }
			break;
    case FSM_EVNT_NAVSWITCH_LEFT:
      edit_screen_move_cursor(&account_screen, LEFT);
      break;
    case FSM_EVNT_NAVSWITCH_RIGHT:
      edit_screen_move_cursor(&account_screen, RIGHT);
      break;
    case FSM_EVNT_NAVSWITCH_UP:
      edit_screen_move_cursor(&account_screen, UP);
      break;
    case FSM_EVNT_NAVSWITCH_DOWN:
      edit_screen_move_cursor(&account_screen, DOWN);
      break;
    case FSM_EVNT_NAVSWITCH_SELECT:
      edit_screen_delete(&account_screen);
      break;
    case FSM_EVNT_KEYPAD_SHARP:
      edit_screen_text_transform(&account_screen);
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      if ((evnt >= FSM_EVNT_KEYPAD_1) && (evnt < FSM_EVNT_KEYPAD_SHARP))
        edit_screen_add(&account_screen, evnt);
    }

  print_edit_screen(&account_screen);
  return FSM_ST_SETTINGS_ACCOUNT;
}

fsm_state_t fsm_st_settings_network(fsm_evnt_t evnt)
{
	static aux = 1;
	static int cursor = 2;

	if(aux){
		drv_lcd_clear_screen();
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "NW SETTINGS");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   2, "DHCP");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   3, "STATIC");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 	4, "SELECT");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 	4, "EXIT");
		aux = 0;
	}

	switch (evnt){
		case FSM_EVNT_NAVSWITCH_UP:
			if(cursor != 2)
				cursor = 2;
			break;
		case FSM_EVNT_NAVSWITCH_DOWN:
			if(cursor != 3)
				cursor = 3;
			break;
    		case FSM_EVNT_GPBUTTON_LEFT:
			if(cursor == 2){
				enable_dhcp();
				lcd_screen_dhcp_enabled(); 
			}
			else{
				cursor = 2;
				aux = 1;
				return FSM_ST_NETWORK_STATIC;
			}
    		case FSM_EVNT_GPBUTTON_RIGHT:
			cursor = 2;
			aux = 1;
			return FSM_ST_MENU_SETTINGS;
    		case FSM_EVNT_NULL:
      			break;
    		default:
      			break;
	}
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,   2, " ");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,   3, " ");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, cursor, "<");
	return FSM_ST_SETTINGS_NETWORK;
}

fsm_state_t fsm_st_network_static(fsm_evnt_t evnt)
{
	extern char *nw_static_edit_fields[NW_STATIC_EDIT_SIZE];
	static alphanumeric_buffer buffer[BUFFER_SIZE(NW_STATIC_EDIT_SIZE)];
	static edit_screen nw_static_screen;
	char dhcp[2];
	static int aux = 1;
	FILE *file;

	if(aux){
		file = fopen(nw_settings, "r+");
		fscanf(file,"DHCP_ENABLED:%s\n",dhcp);
		drv_lcd_cursor(LCD_TOGGLE_ON);
		edit_screen_init_params(&nw_static_screen, buffer, NW_STATIC_EDIT_SIZE, 20, nw_static_edit_fields, file, ipphone_get_nw_static_fields);
		fclose(file);
		aux = 0;
	}

	switch (evnt){
		case FSM_EVNT_GPBUTTON_RIGHT:
			edit_screen_uninit(&nw_static_screen);
			drv_lcd_cursor(LCD_TOGGLE_OFF);
			aux = 1;
	      		return FSM_ST_MENU_SETTINGS;
	    	case FSM_EVNT_GPBUTTON_LEFT:
			if ((buffer[0].buffer[0] != '\0') && (buffer[1].buffer[0] != '\0') && (buffer[2].buffer[0] != '\0') && (buffer[3].buffer[0] != '\0')){
				if(!is_valid_ip(&nw_static_screen)){
					lcd_screen_invalid_ip();
					queue_insert(&event_queue,FSM_EVNT_NULL);
					return FSM_ST_NETWORK_STATIC;		
				}
				apply_static_nw_settings(buffer);
				drv_lcd_cursor(LCD_TOGGLE_OFF);
				edit_screen_uninit(&nw_static_screen);
				aux = 1;
				lcd_screen_static_nw_applied();
				return FSM_ST_MENU_SETTINGS;
			}
			break;
		case FSM_EVNT_NAVSWITCH_LEFT:
			edit_screen_move_cursor(&nw_static_screen, LEFT);
			break;
		case FSM_EVNT_NAVSWITCH_RIGHT:
			edit_screen_move_cursor(&nw_static_screen, RIGHT);
			break;
		case FSM_EVNT_NAVSWITCH_UP:
			edit_screen_move_cursor(&nw_static_screen, UP);
			break;
		case FSM_EVNT_NAVSWITCH_DOWN:
			edit_screen_move_cursor(&nw_static_screen, DOWN);
			break;
		case FSM_EVNT_NAVSWITCH_SELECT:
			edit_screen_delete(&nw_static_screen);
			break;
		case FSM_EVNT_KEYPAD_SHARP:
			edit_screen_text_transform(&nw_static_screen);
			break;
	    case FSM_EVNT_NULL:
	      		break;
	    default:
			if ((evnt >= FSM_EVNT_KEYPAD_1) && (evnt < FSM_EVNT_KEYPAD_SHARP))
				edit_screen_add(&nw_static_screen, evnt);        
	  }

	print_edit_screen(&nw_static_screen);
	return FSM_ST_NETWORK_STATIC;
}

fsm_state_t fsm_st_settings_date_time(fsm_evnt_t evnt)
{
  static int option_index = 0;

  switch (evnt)
  {
    case FSM_EVNT_NAVSWITCH_LEFT:
      option_index = option_index ? (option_index - 1) : (screen_option_qty[SCREEN_DATE_TIME] - 1);
      break;
    case FSM_EVNT_NAVSWITCH_RIGHT:
      option_index = (option_index == (screen_option_qty[SCREEN_DATE_TIME] - 1)) ? 0 : (option_index + 1);
      break;
    case FSM_EVNT_GPBUTTON_LEFT:
      return screen[SCREEN_DATE_TIME].options_map[option_index];
    case FSM_EVNT_GPBUTTON_RIGHT:
      option_index = 0;
      return FSM_ST_MENU_SETTINGS;
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      break;
  }

  lcd_screen_hor_scroll(SCREEN_DATE_TIME, option_index);
  return FSM_ST_SETTINGS_DATE_TIME;
}

fsm_state_t fsm_st_edit_date(fsm_evnt_t evnt)
{
	static int aux = 0, cursor = 0;
	char str[20];
	static int int_month, int_day, int_year;
	int int_min, int_hour;
	char str_month[2], str_day[2], str_min[2], str_hour[2];
	struct tm *tm_ptr;
	time_t t;

	if(!aux)
	{
		t = time((time_t *)0);
		tm_ptr = localtime(&t);
		int_day 	= tm_ptr->tm_mday;
		int_month = tm_ptr->tm_mon + 1;
		int_year 	=	tm_ptr->tm_year + 1900;

		snprintf(str, sizeof(str), "%d/%d/%d", int_day, int_month, int_year);
		drv_lcd_clear_screen();
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, str);
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "DATE");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   4, "SAVE");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,  4, "BACK");
		aux = 1;
	}

	switch (evnt)
	{
		case FSM_EVNT_NAVSWITCH_LEFT:
			cursor = cursor ? (cursor - 1) : 2;
			break;
		case FSM_EVNT_NAVSWITCH_RIGHT:
			cursor = (cursor == 2) ? 0 : (cursor + 1);
			break;
		case FSM_EVNT_NAVSWITCH_UP:
			switch (cursor)
			{
				case 0:
					int_day = (int_day == 31) ? 1 : (int_day + 1);
					break;
				case 1:
					int_month = (int_month == 12) ? 1 : (int_month + 1);
					break;
				case 2:
					int_year = (int_year == 9999) ? 1 : (int_year + 1);
					break;
				default:
					break;
			}
			snprintf(str, sizeof(str), "%d/%d/%d", int_day, int_month, int_year);
			lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, "          ");
			lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, str);
			break;
		case FSM_EVNT_NAVSWITCH_DOWN:
			switch (cursor)
			{
        case 0:
          int_day = (int_day == 1) ? 31 : (int_day - 1);
          break;
        case 1:
          int_month = (int_month == 1) ? 12 : (int_month - 1);
          break;
        case 2:
          int_year = (int_year == 1900) ? 2999 : (int_year - 1);
          break;
        default:
          break;
			}
      snprintf(str, sizeof(str), "%d/%d/%d", int_day, int_month, int_year);
			lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, "          ");
      lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, str);
      break;
		case FSM_EVNT_GPBUTTON_LEFT:
	    t = time((time_t *)0);
	    tm_ptr = localtime(&t);
  	  int_hour = tm_ptr->tm_hour; 
			int_min = tm_ptr->tm_min;

			if (int_day < 10)
				sprintf(str_day, "0%d", int_day);
			else
				sprintf(str_day, "%d", int_day);
      if (int_month < 10)
        sprintf(str_month, "0%d", int_month);
      else
        sprintf(str_month, "%d", int_month);
      if (int_min < 10)
        sprintf(str_min, "0%d", int_min);
      else
        sprintf(str_min, "%d", int_min);
      if (int_hour < 10)
        sprintf(str_hour, "0%d", int_hour);
      else
        sprintf(str_hour, "%d", int_hour);

			snprintf(str, 18, "date %s%s%s%s%d", str_month, str_day, str_hour, str_min, int_year);
			system(str);

			lcd_screen_save();
			aux = 0;
			cursor = 0;
			return FSM_ST_SETTINGS_DATE_TIME;			
		case FSM_EVNT_GPBUTTON_RIGHT:
			aux = 0;
			cursor = 0;
			return FSM_ST_SETTINGS_DATE_TIME;
    case FSM_EVNT_NULL:
      break;
		default:
			break;
	}

	return FSM_ST_EDIT_DATE;
}

fsm_state_t fsm_st_edit_time(fsm_evnt_t evnt)
{
	static int aux = 0, cursor = 0;
	char str[20];
	static int int_min, int_hour;
	int int_month, int_day, int_year;
	char str_month[2], str_day[2], str_min[2], str_hour[2];
	struct tm *tm_ptr;
	time_t t;

	if(!aux)
	{
		t = time((time_t *)0);
		tm_ptr = localtime(&t);
		int_min  = tm_ptr->tm_min;
		int_hour = tm_ptr->tm_hour;

		snprintf(str, sizeof(str), "%d:%d", int_hour, int_min);
		drv_lcd_clear_screen();
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, str);
		lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 1, "TIME");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED,   4, "SAVE");
		lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED,  4, "BACK");
		aux = 1;
	}

	switch (evnt)
	{
		case FSM_EVNT_NAVSWITCH_LEFT:
			cursor = cursor ? (cursor - 1) : 1;
			break;
		case FSM_EVNT_NAVSWITCH_RIGHT:
			cursor = (cursor == 1) ? 0 : (cursor + 1);
			break;
		case FSM_EVNT_NAVSWITCH_UP:
			switch (cursor)
			{
				case 0:
					int_hour = (int_hour == 23) ? 1 : (int_hour + 1);
					break;
				case 1:
					int_min = (int_min == 59) ? 1 : (int_min + 1);
					break;
				default:
					break;
			}
			snprintf(str, sizeof(str), "%d:%d", int_hour, int_min);
			lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, "          ");
			lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, str);
			break;
		case FSM_EVNT_NAVSWITCH_DOWN:
			switch (cursor)
			{
        case 0:
          int_hour = (int_hour == 1) ? 31 : (int_hour - 1);
          break;
        case 1:
          int_min = (int_min == 1) ? 12 : (int_min - 1);
          break;
        default:
          break;
			}
      snprintf(str, sizeof(str), "%d:%d", int_hour, int_min);
			lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, "          ");
      lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, str);
      break;
		case FSM_EVNT_GPBUTTON_LEFT:
	    t = time((time_t *)0);
	    tm_ptr = localtime(&t);
	    int_day   = tm_ptr->tm_mday;
  	  int_month = tm_ptr->tm_mon + 1;
    	int_year  = tm_ptr->tm_year + 1900;

      if (int_day < 10)
        sprintf(str_day, "0%d", int_day);
      else
        sprintf(str_day, "%d", int_day);
      if (int_month < 10)
        sprintf(str_month, "0%d", int_month);
      else
        sprintf(str_month, "%d", int_month);
      if (int_min < 10)
        sprintf(str_min, "0%d", int_min);
      else
        sprintf(str_min, "%d", int_min);
      if (int_hour < 10)
        sprintf(str_hour, "0%d", int_hour);
      else
        sprintf(str_hour, "%d", int_hour);

			snprintf(str, 18, "date %s%s%s%s%d", str_month, str_day, str_hour, str_min, int_year);
			system(str);

			lcd_screen_save();
			aux = 0;
			cursor = 0;
			return FSM_ST_SETTINGS_DATE_TIME;			
		case FSM_EVNT_GPBUTTON_RIGHT:
			aux = 0;
			cursor = 0;
			return FSM_ST_SETTINGS_DATE_TIME;
    case FSM_EVNT_NULL:
      break;
		default:
			break;
	}

	return FSM_ST_EDIT_TIME;
}
static void enable_dhcp(){
	FILE *file;
	char dhcp[4];

	if(file = fopen(nw_settings, "r+")){
		fscanf(file,"DHCP_ENABLED:%s\n",dhcp);
		if(strcmp(dhcp,"n") == 0){
			system("dhcpcd &");
			fseek(file,0,SEEK_SET);
			fprintf(file,"DHCP_ENABLED:y\n");
			fclose(file);
		}
	}
}

static bool_t is_valid_ip(edit_screen *edit){
	int i;
	unsigned long ip = 0;
	alphanumeric_buffer *buffer;
	for(i=0; i < edit->size;i++){
		buffer = edit->vet_buffer + i;
		if(inet_pton(AF_INET, buffer->buffer,&ip) == 0){
			return FALSE;
		}		
	}
	return TRUE;
}
static void apply_static_nw_settings(alphanumeric_buffer *buffer){
	FILE *file;
	char cmd[130];	
	system("pkill dhcpcd");				
	snprintf(cmd,130,"ifconfig eth0 %s netmask %s up;route add default gw %s",buffer[0].buffer, buffer[1].buffer,buffer[2].buffer);
	system(cmd);
	if(file = fopen(nw_settings, "r+")){
		fprintf(file,"DHCP_ENABLED:n\n");
		fprintf(file,"IP_ADDRESS:%s\n",buffer[0].buffer);
		fprintf(file,"NETMASK:%s\n", buffer[1].buffer);
		fprintf(file,"GATEWAY:%s\n", buffer[2].buffer);
		fprintf(file,"DNS_DOMAIN:%s\n", buffer[3].buffer);
		fclose(file);	
	}
}
