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

void fsm_st_idle(fsm_t *fsm, fsm_evnt_t evnt)
{
	printf("ESTADO ATUAL = IDLE\n");
	switch (evnt)
	{
		case FSM_EVNT_RIGHT_BUTTON:
				fsm->state = FSM_ST_IDLE;
				printf("BOTÃO DIREITO APERTADO!");
				break;
		case FSM_EVNT_LEFT_BUTTON:
				fsm->state = FSM_ST_SETTINGS;
				printf("BOTÃO ESQUERDO APERTADO!\n");
				break;
		default:
				printf("PROBLEMA!!!\n");
				break;
	}
}

void fsm_st_settings(fsm_t *fsm, fsm_evnt_t evnt)
{
	printf("ESTADO ATUAL = SETTINGS\n");
	fsm->state = FSM_ST_IDLE;
}
