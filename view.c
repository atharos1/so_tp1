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

    sleep(1);

    sem_t * sem = sem_open(SEM_NAME, O_RDWR);
    int sem_value;
    if (sem == SEM_FAILED) {
        perror("Error opening semaphore");
        exit(EXIT_FAILURE);
    }

    int fd = shm_open(NAME, O_RDWR, 0666);
    if (fd < 0) {
        perror("Error opening shared memory");
        return EXIT_FAILURE;
    }

    sh_mem * shm = (sh_mem *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    shm->status = CONNECTED;

    while((shm->currReadLine < shm->currWriteLine && shm->status != ERROR) || shm->status == CONNECTED) {
        sem_wait(sem);

        printf("%s\n", shm->str[shm->currReadLine]);
        shm->currReadLine++;
    }

    munmap(shm, SHM_SIZE);
    close(fd);
    shm_unlink(NAME);
    sem_close(sem);

}