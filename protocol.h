#ifndef PROTOCOL_H
#define PROTOCOL_H

#define NAME "/so.tp.tp1"
#define SEM_NAME "/so.tp.tp1"

#define FILE_MAX_LENGTH 200
#define MD5_LENGTH 32

#define MAX_FILES 220

typedef enum {
    ERROR,
    AWAITING_CONNECTION,
    CONNECTED,
    FINISHED
} shm_status;

typedef struct sharedMemory {
	char str[MAX_FILES][FILE_MAX_LENGTH + MD5_LENGTH];
	shm_status status;
	int currReadLine;
	int currWriteLine;
} sh_mem;

#define SHM_SIZE sizeof(sh_mem)

#endif