#include "pipemng.h"

int pipe_write(int fd, const char * str) {
  //printf("Escrito: %s\n", str);
  return write(fd, str, strlen(str)+1);
}

int pipe_read(int fd, char * buffer) {
  int i = 0;
  char c;
  //printf("Leido: ");
  while(read(fd, &c, 1) > 0 && c != '\0' && c != '\n') {
    //printf("%c", c);
     buffer[i] = c;
     i++;
  }
  //putchar('\n');
  
  buffer[i] = 0;
  return i;
  /*int fd2 = dup(fd);
  FILE * fp = fdopen(fd2,"r");
  int ret = fscanf(fp, "%s", buffer);
  buffer[1] = 0;
  printf("%s\n", buffer);
  fclose(fp);
  return ret;*/
  /*int ret = read(fd, buffer, 10);
  buffer[10] = 0;
  printf("%s\n", buffer);
  return ret;
  ;*/
}
