#include "pipemng.h"

int pipe_write(int fd, const char * str) {
  //printf("Escrito: %s\n", str);
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

int pipe_write_onebyone(int fd, const char * str, sem_t * semaphore) {

  sem_wait(semaphore);
  int ret = pipe_write(fd, str);
  sem_post(semaphore);

  return ret;

}

int pipe_read_onebyone(int fd, char * buffer, sem_t * semaphore) {
  sem_wait(semaphore);
  int ret = pipe_read(fd, buffer);
  sem_post(semaphore);

  return ret;
};