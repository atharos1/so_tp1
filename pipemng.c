#include "pipemng.h"

int PipeWrite(int fd, const char * str) {
  return write(fd, str, strlen(str)+1);
}

int PipeRead(int fd, char * buffer) {
  int i = 0;
  char c;
  while(read(fd, &c, 1) > 0 && c != '\0' && c != '\n') {
     buffer[i] = c;
     i++;
  }
  
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
