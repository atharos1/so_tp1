#include "view.h"

int main() {
    int shmId;
    sh_mem * shmAddress;

	shmAddress = (sh_mem *) connectWithSharedMemory();

    exit(0);
}

char * connectWithSharedMemory() {
	int shmId;
	char * shmAddress;

	if ((shmId = shmget(shm_key, SHMSZ, 0666)) < 0) {
        perror("Failed to locate shared memory.\n");
        exit(-1);
    }

    if ((shmAddress = shmat(shmId, NULL, 0)) == (char *) -1) {
        perror("Failed to connect with data space.\n");
        exit(-1);
    }

    return shmAddress;
}