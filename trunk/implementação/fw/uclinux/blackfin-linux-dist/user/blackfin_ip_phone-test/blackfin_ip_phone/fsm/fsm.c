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
{/*
  switch (evnt)
  {
    case
    case
    case FSM_EVNT_NULL:
      return FSM_ST_MENU;
    default:
      fsm_error(FSM_ST_MENU);
  }
*/
}

fsm_state_t fsm_st_menu_contacts(fsm_evnt_t evnt)
{
}

fsm_state_t fsm_st_menu_call_logs(fsm_evnt_t evnt)
{
}

fsm_state_t fsm_st_menu_settings(fsm_evnt_t evnt)
{
}

void fsm_error(fsm_state_t state)
{
	printf("ERROR!\n");
	printf("State: %d\n", state);
	exit(1);
}
