				/*
 * Project: BLACKFIN IP PHONE
 * 
 * fsm.c - Finite state machine.
 */

#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "queue.h"

void fsm_init(fsm_t *fsm)
{
	fsm->state = FSM_ST_IDLE;
	fsm->function[FSM_ST_IDLE]						= fsm_st_idle;
	fsm->function[FSM_ST_MENU] 						= fsm_st_menu;
	fsm->function[FSM_ST_MENU_CONTACTS]		=	fsm_st_menu_contacts;
	fsm->function[FSM_ST_MENU_CALL_LOGS]	= fsm_st_menu_call_logs;
	fsm->function[FSM_ST_MENU_SETTINGS]		= fsm_st_menu_settings;
}

fsm_state_t fsm_st_idle(fsm_evnt_t evnt)
{
	switch (evnt)
	{
		case FSM_EVNT_GPBUTTON_RIGHT:
			return FSM_ST_IDLE;
		case FSM_EVNT_GPBUTTON_LEFT:
			printf("estado atual = MENU\n");
			return FSM_ST_MENU;
		case FSM_EVNT_NULL:
			return FSM_ST_IDLE;
		default:
			return FSM_ST_IDLE;
	}
}

fsm_state_t fsm_st_menu(fsm_evnt_t evnt)
{
  switch (evnt)
  {
    case FSM_EVNT_GPBUTTON_RIGHT:
			printf("estado atual = IDLE\n");
			return FSM_ST_IDLE;
    case FSM_EVNT_GPBUTTON_LEFT:
			return FSM_ST_IDLE;
		case FSM_EVNT_NULL:
			return FSM_ST_IDLE;
    default:
			return FSM_ST_IDLE;
  }
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
