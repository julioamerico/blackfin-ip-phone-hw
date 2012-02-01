/*
 *	Project: BLACKFIN IP PHONE
 * 
 *	queue.c - Event queue.
 *
 *	An event queue implemented as a linked list.
 *
 *	References:
 *		- Pointers On C, Kenneth A. Reek.
 *		- http://www.cs.bu.edu/teaching/c/queue/linked-list/
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "fsm.h"
#include "queue.h"

/*
**	Returns a queue that has only one node with its pointers properly initialized.
*/
void queue_init(queue_t *queue)
{
	queue->pqueue_front = NULL;
	queue->pqueue_back	= NULL;
}

/*
**	Adds a new event to back of the queue.
*/
void queue_insert(queue_t *queue, fsm_evnt_t new_event)
{
	queue_node_t	*new_node;

	if ((new_node = (queue_node_t*)malloc(sizeof(queue_node_t))) == NULL)
	{
		printf("ERROR (queue.c) - problems with memory allocation\n");
	}
	else
	{
    new_node->event = new_event;
    new_node->next_node = NULL;
		queue->pqueue_back = new_node;

		if (queue_is_empty(queue)) /* queue is empty */
		{
			queue->pqueue_front = new_node;
		}
		else
		{
			queue->pqueue_back->next_node = new_node;
		}
	}
}

/*
**	Removes an event from front of the queue, discarding it.
*/
void queue_delete(queue_t *queue)
{
	queue_node_t *pointer;

	if (!queue_is_empty(queue))
	{
		if (queue->pqueue_front->next_node == NULL) /* last element in queue */
		{
		  queue->pqueue_front = NULL;
  		queue->pqueue_back  = NULL;
		}
		else
		{
			pointer = queue->pqueue_front->next_node;
			free(queue->pqueue_front);
			queue->pqueue_front = pointer;
		}
	}
}

/*
**	Returns an event that is in front of the queue without changing the queue.
*/
fsm_evnt_t queue_read(queue_t *queue)
{
	fsm_evnt_t ev = FSM_EVNT_NULL;

	if (!queue_is_empty(queue))
	{
		ev = queue->pqueue_front->event;
	}

	return ev;
}

/*
**	Returns TRUE if the queue is empty, else FALSE.
*/
int queue_is_empty(queue_t *queue)
{
	return (queue->pqueue_front == NULL);
}
