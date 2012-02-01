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
	fsm->function[FSM_ST_IDLE]				= fsm_st_idle;
	fsm->function[FSM_ST_SETTINGS] 		= fsm_st_settings;
}

fsm_state_t fsm_st_idle(fsm_state_t st, fsm_evnt_t evnt)
{
	printf("ESTADO ATUAL = IDLE\n");
	switch (evnt)
	{
		case FSM_EVNT_RIGHT_BUTTON:
				printf("BOTÃO DIREITO APERTADO!");
				return (FSM_ST_IDLE);
				break;
		case FSM_EVNT_LEFT_BUTTON:
				printf("BOTÃO ESQUERDO APERTADO!\n");
				return (FSM_ST_SETTINGS);
				break;
		default:
				printf("PROBLEMA!!!\n");
				break;
	}
}

fsm_state_t fsm_st_settings(fsm_state_t st, fsm_evnt_t evnt)
{
	printf("ESTADO ATUAL = SETTINGS\n");
	return (FSM_ST_IDLE);
}
