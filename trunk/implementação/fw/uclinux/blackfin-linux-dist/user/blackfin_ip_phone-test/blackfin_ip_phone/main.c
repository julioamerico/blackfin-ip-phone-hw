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

	//LinphoneProxyConfig *cfg;

	lcd_init();
	expander_init();
	fsm_init(&fsm);
	queue_init(&event_queue);
	ipphone_init(&ipphone_core, NULL);

/*
	ipphone_set_passwd("120887");
	cfg = ipphone_proxy_config_new();
	ipphone_proxy_config_set_server_addr(cfg, "sip:10.1.1.114@10.1.1.114");
	ipphone_proxy_config_set_identity(cfg, "sip:1002@10.1.1.114");
	ipphone_proxy_config_enableregister(cfg, TRUE);
	ipphone_add_proxy_config(&ipphone_core, cfg);

	ipphone_add_friend(&ipphone_core, "julio <sip:romulo@david>");
*/

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
