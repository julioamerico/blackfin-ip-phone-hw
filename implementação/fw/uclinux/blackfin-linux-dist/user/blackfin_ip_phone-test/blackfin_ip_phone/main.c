/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	main.c - Main application file.
 */

#include <stdio.h>
#include <stdlib.h>

#include "fsm/queue.h"
#include "drivers/event/expander.h"

int main (void)
{
	fsm_t fsm;
	queue_t	event_queue;

	fsm_init(&fsm);
	queue_init(&event_queue);
	expander_init();

	while(1)
	{
		int ev_qty;
		
    fsm_evnt_t		fsm_event;
	  fsm_state_t		fsm_state;
/*
		usleep(20000);
*/
		ev_qty = expander_read(&event_queue);

		if (ev_qty)
		{
    	fsm_event = queue_read(&event_queue);
    	queue_delete(&event_queue);

  		fsm_state = fsm.state;
			fsm.state = fsm.function[fsm_state](fsm_event);
		}

	}
}







/*
	#include <string.h>	

	printf("O programa está sendo executado!!\n");

  lcd_init();

	drv_lcd_clear_screen();

	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 1, "XUXA M.");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 1, "1002");
  lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 2, "17:05");
	lcd_write_justified(LCD_WRITE_CENTER_JUSTIFIED, 3, "SUN, 10/05/2012");
	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 4, "MENU");
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, 4, "CONTACTS");

  drv_lcd_close();

	return 0;
}*/
