#include "pipemng.h"

int pipe_write(int fd, const char * str) {
  return write(fd, str, strlen(str)+1);
}

int pipe_read(int fd, char * buffer) {
  int i = 0;
  char c;
  while(read(fd, &c, 1) > 0 && c != '\0' && c != '\n') {
     buffer[i] = c;
     i++;
  }
  
  buffer[i] = 0;
  return i;
}

//Funcion que espera a que el fd no se encuentre accedido y lee. Luego libera el recurso.
//Se presume semaphore inicializado en uno, y oscilará entre uno y 0 segun el recurso se encuentre o no en uso.
int pipe_read_onebyone(int fd, char * buffer, sem_t * semaphore) {
  sem_wait(semaphore);
  int ret = pipe_read(fd, buffer);
  sem_post(semaphore);

  return ret;
}
