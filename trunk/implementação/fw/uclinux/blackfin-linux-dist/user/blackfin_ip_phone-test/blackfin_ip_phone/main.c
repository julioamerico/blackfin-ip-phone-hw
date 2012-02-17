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

int main (void)
{
	fsm_t fsm;
	queue_t	event_queue;

	lcd_init();
	expander_init();
	fsm_init(&fsm);
	queue_init(&event_queue);

	while(1)
	{
		int ev_qty;
		
    fsm_evnt_t		fsm_event;
	  fsm_state_t		fsm_state;

		usleep(20000);

		ev_qty = expander_read(&event_queue);
		
		if (ev_qty)
		{
	   	fsm_event = queue_read(&event_queue);
  	 	queue_delete(&event_queue);

 			fsm_state = fsm.state;
			fsm.state = fsm.function[fsm_state](fsm_event);

			if (fsm_state != fsm.state)
				queue_insert(&event_queue, FSM_EVNT_NULL);
		}
	}
}
