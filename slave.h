#ifndef _SLAVE_H
#define _SLAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ID 57526
#define MASTER_QUEUE_ID 1
#define SLAVE_QUEUE_ID 2
#define BUFFER_SIZE (FILENAME_MAX + MD5_LENGTH)

int calculate_MD5(const char * path, char * buffer);

#endif
