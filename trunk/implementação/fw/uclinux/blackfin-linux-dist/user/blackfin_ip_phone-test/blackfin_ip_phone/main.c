/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	main.c - Main application file.
 */

#include <stdio.h>
#include <stdlib.h>

#include "fsm/queue.h"
#include "drivers/event/expander.h"
#include "drivers/lcd/lcd.h"
#include "ipphone.h"

LinphoneCore ipphone_core;
main_queue_t  event_queue;

int main (void)
{
	fsm_t fsm;

	lcd_init();
	expander_init();
	queue_init(&event_queue);
	ipphone_init(&ipphone_core, &event_queue);
	fsm_init(&fsm);

	while(1)
	{		
    fsm_evnt_t		fsm_event;
	  fsm_state_t		fsm_state;

		usleep(20000);

		ipphone_iterate(&ipphone_core);

		expander_read(&event_queue);
		fsm_event = queue_read(&event_queue);

		if (fsm_event != FSM_EVNT_QUEUE_IS_EMPTY)
		{
  	 	queue_delete(&event_queue);

 			fsm_state = fsm.state;
			fsm.state = fsm.function[fsm_state](fsm_event);

			if (fsm_state != fsm.state)
				queue_insert(&event_queue, FSM_EVNT_NULL);
		}
	}
}
