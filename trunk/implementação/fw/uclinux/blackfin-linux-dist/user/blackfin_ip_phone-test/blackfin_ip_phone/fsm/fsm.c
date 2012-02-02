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
	printf("FILA DE EVENTOS INICIALIZADA COM SUCESSO\n");
	fsm->state = FSM_ST_IDLE;
	fsm->function[FSM_ST_IDLE]					= fsm_st_idle;
	fsm->function[FSM_ST_SETTINGS] 			= fsm_st_settings;
	fsm->function[FSM_ST_SPEED_DIAL]		=	fsm_st_speed_dial;
	fsm->function[FSM_ST_CONTACT_LIST]	= fsm_st_contact_list;
	fsm->function[FSM_ST_LOST_CALLS]		= fsm_st_lost_calls;
}

fsm_state_t fsm_st_idle(fsm_evnt_t evnt)
{
	printf("ESTADO ATUAL = IDLE\n");
	switch (evnt)
	{
		case FSM_EVNT_KEYPAD:
				printf("evnt_keypad executado!\n");
				return (FSM_ST_SETTINGS);
				break;
		case FSM_EVNT_JOYSTICK:
				break;
		case FSM_EVNT_RIGHT_BUTTON:
				break;
		case FSM_EVNT_LEFT_BUTTON:
				break;
		case FSM_EVNT_SD_DETECT:
				break;
		case FSM_EVNT_NULL:
				return (FSM_ST_IDLE);
				break;
		default:
				return (FSM_ST_IDLE);
				break;
	}
}

fsm_state_t fsm_st_settings(fsm_evnt_t evnt)
{
  printf("ESTADO ATUAL = SETTINGS\n");
  switch (evnt)
  {
    case FSM_EVNT_KEYPAD:
        break;
    case FSM_EVNT_JOYSTICK:
				printf("evnt_joystick executado!\n");
				return (FSM_ST_SPEED_DIAL);
        break;
    case FSM_EVNT_RIGHT_BUTTON:
        break;
    case FSM_EVNT_LEFT_BUTTON:
        break;
    case FSM_EVNT_SD_DETECT:
        break;
    case FSM_EVNT_NULL:
				return (FSM_ST_IDLE);
        break;
    default:
				return (FSM_ST_IDLE);
        break;
  }
}

fsm_state_t fsm_st_speed_dial(fsm_evnt_t evnt)
{
  printf("ESTADO ATUAL = SPEED DIAL\n");
  switch (evnt)
  {
    case FSM_EVNT_KEYPAD:
        break;
    case FSM_EVNT_JOYSTICK:
        break;
    case FSM_EVNT_RIGHT_BUTTON:
				printf("evnt_right_button executado!\n");
				return (FSM_ST_CONTACT_LIST);
        break;
    case FSM_EVNT_LEFT_BUTTON:
        break;
    case FSM_EVNT_SD_DETECT:
        break;
    case FSM_EVNT_NULL:
        return (FSM_ST_IDLE);
        break;
    default:
        return (FSM_ST_IDLE);
        break;
  }
}

fsm_state_t fsm_st_contact_list(fsm_evnt_t evnt)
{
  printf("ESTADO ATUAL = CONTACT LIST\n");
  switch (evnt)
  {
    case FSM_EVNT_KEYPAD:
        break;
    case FSM_EVNT_JOYSTICK:
        break;
    case FSM_EVNT_RIGHT_BUTTON:
        break;
    case FSM_EVNT_LEFT_BUTTON:
				printf("evnt_left_button executado!\n");
				return (FSM_ST_LOST_CALLS);
        break;
    case FSM_EVNT_SD_DETECT:
        break;
    case FSM_EVNT_NULL:
        return (FSM_ST_IDLE);
        break;
    default:
        return (FSM_ST_IDLE);
        break;
  }
}

fsm_state_t fsm_st_lost_calls(fsm_evnt_t evnt)
{
  printf("ESTADO ATUAL = LOST CALLS\n");
  switch (evnt)
  {
    case FSM_EVNT_KEYPAD:
        break;
    case FSM_EVNT_JOYSTICK:
        break;
    case FSM_EVNT_RIGHT_BUTTON:
        break;
    case FSM_EVNT_LEFT_BUTTON:
        break;
    case FSM_EVNT_SD_DETECT:
				printf("evnt_sd_detect executado!\n");
				return (FSM_ST_IDLE);
        break;
    case FSM_EVNT_NULL:
        return (FSM_ST_IDLE);
        break;
    default:
        return (FSM_ST_IDLE);
        break;
  }
}
