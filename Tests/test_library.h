#ifndef _TEST_LIBRARY_H
#define _TEST_LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void given_nothing();
void check_is_not_null(void * pointer);
void check_strings_equal(char * s1, char * s2);
void then_success();
void passed();
void failed(char * error);
void not_implemented();

#endif