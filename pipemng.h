#ifndef _PIPEMNG_H
#define _PIPEMNG_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>

int PipeWrite(int fd, char * str);
int PipeRead(int fd, char * buffer);

#endif
