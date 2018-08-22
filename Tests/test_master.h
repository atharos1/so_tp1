#ifndef _TEST_MASTER_H
#define _TEST_MASTER_H

#include "../master.h"
#include "../pipemng.h"
#include "test_library.h"

int master_test_run();
void when_creating_pipe();
void then_pipe_is_created();
void when_adding_item_to_pipe();
void then_pipe_is_not_empty();

#endif