#ifndef _SLAVE_H
#define _SLAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MD5_LENGTH 32
#define BUFFER_SIZE 900
#define READ 0
#define WRITE 1

int calculate_MD5(const char * path, char * buffer);

#endif
