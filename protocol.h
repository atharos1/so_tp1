#ifndef PROTOCOL_H
#define PROTOCOL_H

#define NAME "/so.tp.tp1"
#define SEM_NAME "/so.tp.tp1"

#define BUFFER_SIZE 100
#define BUFFER_ROWS 30

typedef struct sharedMemory {
	char str[BUFFER_ROWS][BUFFER_SIZE];
	int status;
	int currReadLine;
	int currWriteLine;
} sh_mem;

#define SIZE sizeof(sh_mem)

#endif