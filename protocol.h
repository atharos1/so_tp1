#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>

#define NAME "/so.tp.tp1" //Shared memory ID
#define SEM_NAME "/so.tp.tp1" //View's semaphore ID

#define SLAVE_SEM_NAME "/so.tp.tp1.slave"

#define FILE_MAX_LENGTH 200
#define MD5_LENGTH 32

#define MAX_FILES 220

typedef enum { //Connection master-slave states
    ERROR,
    AWAITING_CONNECTION,
    CONNECTED,
    FINISHED
} shm_status;

typedef struct sharedMemory { //Shared memory structure
	char str[MAX_FILES][FILE_MAX_LENGTH + MD5_LENGTH];
	shm_status status;
	int currReadLine;
	int currWriteLine;
} sh_mem;

#define SHM_SIZE sizeof(sh_mem)

#endif