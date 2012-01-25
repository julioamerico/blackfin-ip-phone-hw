/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	main.c - Main application file.
 */

#include <stdio.h>
#include <stdlib.h>

#include "fsm/fsm.h"
#include "fsm/queue.h"

int main (void)
{
	fsm_t fsm;
	queue_t event_queue;
	
	fsm_init(&fsm);
	queue_init(&event_queue);

  while (1)
	{
		fsm_evnt_t	event;
		
		queue_insert(&event_queue, FSM_EVNT_RIGHT_BUTTON); /* TESTE! */

		event = queue_read(&event_queue);
		queue_delete(&event_queue);

		fsm.function[fsm.state](&fsm, event);
	}
}
