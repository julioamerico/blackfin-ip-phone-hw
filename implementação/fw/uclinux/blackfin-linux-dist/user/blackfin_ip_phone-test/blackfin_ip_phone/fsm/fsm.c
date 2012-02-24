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

extern int screen_option_qty[MAX_HOR_SCREENS];
extern hor_scroll_screen_fields_t screen[MAX_HOR_SCREENS];

void fsm_init(fsm_t *fsm)
{
	fsm->state = FSM_ST_IDLE;
	fsm->function[FSM_ST_IDLE]						= fsm_st_idle;
	fsm->function[FSM_ST_MENU] 						= fsm_st_menu;
	fsm->function[FSM_ST_MENU_CONTACTS]		=	fsm_st_menu_contacts;
	fsm->function[FSM_ST_MENU_CALL_LOGS]	= fsm_st_menu_call_logs;
	fsm->function[FSM_ST_MENU_SETTINGS]		= fsm_st_menu_settings;

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
			return FSM_ST_CONTACTS_LIST;
		case FSM_EVNT_NULL:
			return FSM_ST_IDLE;
		default:
			fsm_error(FSM_ST_IDLE);
	}
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
			break;
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
      break;
    case FSM_EVNT_NULL:
      break;
    default:
      fsm_error(FSM_ST_MENU_CONTACTS);
  }

  lcd_screen_hor_scroll(SCREEN_CONTACTS, option_index);

  return FSM_ST_MENU_CONTACTS;
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
