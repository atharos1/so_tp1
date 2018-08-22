#ifndef _PIPEMNG_H
#define _PIPEMNG_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <semaphore.h>

int pipe_write(int fd, const char * str);
int pipe_read(int fd, char * buffer);

int pipe_write_onebyone(int fd, const char * str, sem_t * semaphore); //Requiere semaforo en 1 si habilitado a escribir
int pipe_read_onebyone(int fd, char * buffer, sem_t * semaphore); //Requiere semaforo en 1 si habilitado a leer

#endif
