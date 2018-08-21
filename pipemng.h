#ifndef _PIPEMNG_H
#define _PIPEMNG_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>

int pipe_write(int fd, const char * str);
int pipe_read(int fd, char * buffer);

#endif
