/*
 * Project: BLACKFIN IP PHONE
 * 
 * fsm.c - Finite state machine.
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <strings.h>

void fsm_init(fsm_t *fsm)
{
	fsm->state = FSM_ST_IDLE;
	fsm->function[FSM_ST_IDLE]				= fsm_st_idle;
	fsm->function[FSM_ST_SPEED_DIAL]  = fsm_speed_dial;
	fsm->function[FSM_ST_SETTINGS] 		= fsm_st_settings;

	for (int i = 0; i < fsm->event_buffer.buffer_size; i++)
	{
		fsm->event_buffer.buffer[i] = FSM_EVNT_NULL;
	}
}

void fsm_write_buffer(fsm_evnt_t event, evnt_buffer_t *event_buffer)
{
	event_buffer->index = (event_buffer->index++ % buffer_size);
	event_buffer->buffer[event_buffer->index] = event;
}

void fsm_st_idle(fsm_evnt_t evnt)
{
}

void fsm_st_speed_dial(fsm_evnt_t evnt)
{
}

void fsm_st_settings(fsm_evnt_t evnt)
{
}







