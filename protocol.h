#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>

#define NAME "/so.tp.tp1" //Id de la memoria compartida
#define SEM_NAME "/so.tp.tp1" //Id semaforo de la view

#define SLAVE_SEM_NAME "/so.tp.tp1.slave" //NO SE SI VA ACA

#define FILE_MAX_LENGTH 200
#define MD5_LENGTH 32

#define MAX_FILES 220

typedef enum { //Estados de la conexion master-view
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
} sh_mem; //Estructura de memoria compartida

#define SHM_SIZE sizeof(sh_mem)

#endif