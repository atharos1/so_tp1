//#include "shm.h"
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {

    sleep(1);

    sem_t * sem = sem_open(SEM_NAME, O_RDWR); //Semaphore to avoid active wait while waiting for new elements to print

    if (sem == SEM_FAILED) {
        printf("Error opening semaphore.\n\nExiting program..\n");
        exit(EXIT_FAILURE);
    }

    int fd_shm = shm_open(NAME, O_RDWR, 0666);
    if (fd_shm < 0) {
        printf("Error opening shared memory.\n\nExiting program..\n");
        exit(EXIT_FAILURE);
    }

    sh_mem * shm = (sh_mem *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0); //Pointer to shared structure

    shm->status = CONNECTED; //Let know master that view was connected

    while((shm->currReadLine < shm->currWriteLine && shm->status != ERROR) || shm->status == CONNECTED) { //While master is working or there are pending elements in buffer
        sem_wait(sem);//Avoid active wait. Semaphore value corresponds to number of pending elements in buffer to print

        printf("%s\n", shm->str[shm->currReadLine]);
        shm->currReadLine++;
    }

    //Free resources in view instead of master because view finishes after master
    munmap(shm, SHM_SIZE);
    close(fd_shm);
    shm_unlink(NAME);
    sem_close(sem);

}