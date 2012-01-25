/*
 * Project: BLACKFIN IP PHONE
 * 
 * queue.h - Event queue.
 */

#include "fsm.h"

/*
**  Structure to hold event and pointer for the next element on the queue.
*/
typedef struct queue_node {
  fsm_event_t event;
  struct queue_node *next_node;
} queue_node_t;

/*
**	Structure that holds pointers to front and back of the queue, and
**	represents the queue itself.
*/
typedef struct queue_pointers {
	queue_node_t *pqueue_front;
	queue_node_t *pqueue_back;
} queue_t;
