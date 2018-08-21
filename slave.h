#ifndef _SLAVE_H
#define _SLAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "protocol.h"

#define READ 0
#define WRITE 1

void calculate_MD5(const char * path, char * buffer);

#endif
