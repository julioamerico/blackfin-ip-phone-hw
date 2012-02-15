#ifndef BLACKFIN_IP_PHONE_EXPANDER_H
#define BLACKFIN_IP_PHONE_EXPANDER_H

#include "../../fsm/fsm.h"
#include "../../fsm/queue.h"

void expander_init(void);
int expander_read(queue_t *queue);

#endif
