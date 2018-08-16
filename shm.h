#define SHMSZ 100
#define BUFFER_SIZE 100

key_t shm_key = 31;

typedef struct sharedMemory {
	char str[BUFFER_SIZE];
	int currReadLine;
	int currWriteLine;
} sh_mem;