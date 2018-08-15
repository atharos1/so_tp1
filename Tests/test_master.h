#ifndef _TEST_MASTER_H
#define _TEST_MASTER_H

#include "../master.h"
#include "../sysvmq.h"
#include "test_library.h

int test_run();
void when_creating_queue();
void then_queue_is_created();
void when_adding_item_to_queue();
void then_queue_is_not_empty();

#endif