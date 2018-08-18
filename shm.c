#include "shm.h"

long connectWithSharedMemory(long key, long size, void * buffer) {
	int shmId;

	if ((shmId = shmget(key, size, 0666)) < 0) {
        perror("Failed to locate shared memory.\n");
        exit(-1);
    }

    if ((buffer = shmat(shmId, NULL, 0)) == (char *) -1) {
        perror("Failed to connect with data space.\n");
        exit(-1);
    }

    return shmId;
}

long createSharedMemory(long key, long size, void * buffer) {
    int shmId;

    if ((shmId = shmget(25550, size, IPC_CREAT | 0666)) < 0) {
        perror("Failed to create shared memory.\n");
        exit(-1);
    }

    if ((buffer = shmat(shmId, NULL, 0)) == (char *) -1) {
        perror("Failed to attach segment to data space.\n");
        exit(-1);
    }

    return shmId;
}

void disconnectFromSharedMemory(void * buffer) {
    shmdt((void *) buffer);
}

void destroySharedMemory(long shmID) {
        shmctl(shmID, IPC_RMID, NULL);
}