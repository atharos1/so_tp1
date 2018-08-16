#include "view.h"

int main() {
    int shmId;
    key_t key;
    sh_mem * shmAddress;

	scanf("%d", &key);
	printf("Parent PID: %d\n", key); // Da cero...

	shmAddress = (sh_mem *) connectWithSharedMemory(key);
	printf("Connected with shared memory succesfully\n");

    exit(0);
}

char * connectWithSharedMemory(key_t key) {
	int shmId;
	char * shmAddress;

	if ((shmId = shmget(key, SHMSZ, 0666)) < 0) {
        perror("Failed to locate shared memory.\n");
        exit(-1);
    }

    if ((shmAddress = shmat(shmId, NULL, 0)) == (char *) -1) {
        perror("Failed to connect with data space.\n");
        exit(-1);
    }

    return shmAddress;
}