#ifndef _SHM_H
#define _SHM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SHMSZ 100
#define BUFFER_SIZE 100
#define BUFFER_ROWS 30

//key_t shm_key = 31;

typedef struct sharedMemory {
	char str[BUFFER_ROWS][BUFFER_SIZE];
	int status;
	int currReadLine;
	int currWriteLine;
} sh_mem;

long createSharedMemory(long key, long size, void * buffer);
long connectWithSharedMemory(long key, long size, void * buffer);
void disconnectFromSharedMemory(void * buffer);
void destroySharedMemory(long shmID);

#endif