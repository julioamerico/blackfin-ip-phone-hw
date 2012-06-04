#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <linux/version.h>
#include <linux/input.h>

#include "expander.h"

#define PATH_EVNT	"/dev/input/event0"

#ifndef EV_SYN
#define EV_SYN 0
#endif

static int fd;

void expander_init(void)
{
	if ((fd = open(PATH_EVNT, O_NONBLOCK | O_RDONLY)) < 0)
	{
		printf("ERROR: can't find /dev/input/event0\n");
	}
}

int expander_read(main_queue_t *queue)
{
	struct input_event ev[64];
	int rd, index;
	int ev_qty = 0;
	fsm_evnt_t pressed_button;

	rd = read(fd, ev, sizeof(struct input_event) * 64);

	if (rd > 0)
	{
		for (index = 0; index < rd / sizeof(struct input_event); index++)
		{
			if ((ev[index].type != EV_SYN) && (ev[index].value == 1))
			{
				switch (ev[index].code)
				{
					/* Keypad Buttons */
					case KEY_F1:
						pressed_button = FSM_EVNT_KEYPAD_1;
						break;
					case KEY_F2:
						pressed_button = FSM_EVNT_KEYPAD_2;
            break;
					case KEY_F3:
            pressed_button = FSM_EVNT_KEYPAD_3;
            break;
					case KEY_F4:
            pressed_button = FSM_EVNT_KEYPAD_4;
            break;
					case KEY_F5:
            pressed_button = FSM_EVNT_KEYPAD_5;
            break;
					case KEY_F6:
            pressed_button = FSM_EVNT_KEYPAD_6;
            break;
					case KEY_F7:
            pressed_button = FSM_EVNT_KEYPAD_7;
            break;
					case KEY_F8:
            pressed_button = FSM_EVNT_KEYPAD_8;
            break;
					case KEY_F9:
            pressed_button = FSM_EVNT_KEYPAD_9;
            break;
					case KEY_KPASTERISK:
            pressed_button = FSM_EVNT_KEYPAD_ASTERISK;
            break;
					case KEY_F10:
            pressed_button = FSM_EVNT_KEYPAD_0;
            break;
					case KEY_DOT:
            pressed_button = FSM_EVNT_KEYPAD_SHARP;
            break;
					/* Navigation Switch */
					case KEY_1:
            pressed_button = FSM_EVNT_NAVSWITCH_LEFT;
						break;
					case KEY_2:
            pressed_button = FSM_EVNT_NAVSWITCH_RIGHT;
						break;
					case KEY_3:
            pressed_button = FSM_EVNT_NAVSWITCH_DOWN;
						break;
					case KEY_4:
            pressed_button = FSM_EVNT_NAVSWITCH_UP;
						break;
					case KEY_5:
            pressed_button = FSM_EVNT_NAVSWITCH_SELECT;
						break;
					/* GP Buttons */
					case KEY_6:
            pressed_button = FSM_EVNT_GPBUTTON_RIGHT;
						break;
					case KEY_7:
            pressed_button = FSM_EVNT_GPBUTTON_LEFT;
						break;
					default:
						break;
				}
				queue_insert(queue, pressed_button);
        ev_qty++;
			}
			
		}
		return ev_qty;
	}
	else
	{
		return 0;
	}
}


