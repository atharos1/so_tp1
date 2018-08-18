//#include "shm.h"
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>

int main() {

    sem_t * sem = sem_open(SEM_NAME, O_RDWR);
    int sem_value;
    if (sem == SEM_FAILED) {
        perror("Error abriendo semaforo");
        exit(EXIT_FAILURE);
    }

    int fd = shm_open(NAME, O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open()");
        return EXIT_FAILURE;
    }

    sh_mem * shm = (sh_mem *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    

    while(shm->currReadLine < shm->currWriteLine || shm->status == 1) {
        sem_wait(sem);

        printf("%s\n", shm->str[shm->currReadLine]);
        shm->currReadLine++;
    }

    munmap(shm, SIZE);
    close(fd);
    shm_unlink(NAME);

}