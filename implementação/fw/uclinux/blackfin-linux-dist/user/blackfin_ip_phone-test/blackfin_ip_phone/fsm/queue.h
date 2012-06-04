/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	queue.h - Event queue.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "fsm.h"

/*
**  Structure to hold event and pointer for the next element on the queue.
*/
typedef struct queue_node {
  fsm_evnt_t event;
  struct queue_node *next_node;
} queue_node_t;

/*
**	Structure that holds pointers to front and back of the queue, and
**	represents the queue itself.
*/
typedef struct queue_pointers {
	queue_node_t *pqueue_front;
	queue_node_t *pqueue_back;
} main_queue_t;

/*
**	Function prototypes
*/
void queue_init(main_queue_t *queue);
void queue_insert(main_queue_t *queue, fsm_evnt_t new_event);
void queue_delete(main_queue_t *queue);
fsm_evnt_t queue_read(main_queue_t *queue);
int queue_is_empty(main_queue_t *queue);

#endif
