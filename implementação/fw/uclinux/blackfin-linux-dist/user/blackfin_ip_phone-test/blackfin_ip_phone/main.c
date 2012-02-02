/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	main.c - Main application file.
 */

#include <stdio.h>
#include <stdlib.h>

#include "fsm/queue.h"

int main (void)
{
	fsm_t fsm;
	queue_t event_queue;
	
	fsm_init(&fsm);
	queue_init(&event_queue);	

	queue_insert(&event_queue, FSM_EVNT_KEYPAD); /* TESTE! */
	queue_insert(&event_queue, FSM_EVNT_JOYSTICK); /* TESTE! */
	queue_insert(&event_queue, FSM_EVNT_RIGHT_BUTTON); /* TESTE! */
	queue_insert(&event_queue, FSM_EVNT_LEFT_BUTTON); /* TESTE! */
	queue_insert(&event_queue, FSM_EVNT_SD_DETECT); /* TESTE! */

  while (1)
	{
		fsm_evnt_t	fsm_event;
		fsm_state_t fsm_state;

		fsm_event = queue_read(&event_queue);
		queue_delete(&event_queue);
		
		fsm_state = fsm.state;
		fsm.state = fsm.function[fsm_state](fsm_event);
	}
}
