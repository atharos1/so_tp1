//#include "shm.h"
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {

    sleep(1);

    sem_t * sem = sem_open(SEM_NAME, O_RDWR); //Semaforo para evitar espera activa mientras se esperan nuevos elementos a imprimir

    if (sem == SEM_FAILED) {
        perror("Error opening semaphore");
        exit(EXIT_FAILURE);
    }

    int fd_shm = shm_open(NAME, O_RDWR, 0666);
    if (fd_shm < 0) {
        perror("Error opening shared memory");
        return EXIT_FAILURE;
    }

    sh_mem * shm = (sh_mem *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0); //Puntero a la estructura compartida

    shm->status = CONNECTED; //Aviso al master que me conecté

    while((shm->currReadLine < shm->currWriteLine && shm->status != ERROR) || shm->status == CONNECTED) { //Mientras el master este trabajando o mientras tenga cosas pendientes en el buffer
        sem_wait(sem);//Evito espera activa. Valor del semaforo corresponde a cantidad de elementos en buffer pendientes de impresion

        printf("%s\n", shm->str[shm->currReadLine]);
        shm->currReadLine++;
    }

    //Liberamos recursos nosotros en vez del master porque terminaremos después
    munmap(shm, SHM_SIZE);
    close(fd_shm);
    shm_unlink(NAME);
    sem_close(sem);

}