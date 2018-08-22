#ifndef _TEST_LIBRARY_H
#define _TEST_LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_is_not_null(void * pointer);
void passed();
void failed(char * error);

#endif