#ifndef _TEST_SLAVE_H
#define _TEST_SLAVE_H

#include "../slave.h"
#include "test_library.h"

int test_run();
void test_MD5_hashing();
void given_existing_file();
void when_calculating_hash();
void then_return_value_is_not_null();

#endif