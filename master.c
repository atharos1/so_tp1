#include "master.h"
#include "pipemng.h"

//int queue_id;
int fd1[2];
int fd2[2];

int main(int argc, const char ** argv) {
    if (argc <= 1) { //Argument zero is the program name
        perror("Error. Program should receive at least one argument.\n\nExiting program..\n");
        exit(-1);
    } else {
        printf("\nHashing starting..\n\n");
        run(argc,argv);
    }
}

void run(int argc, const char ** argv) {
    int parameters_offset = 1;
    int number_files = argc - parameters_offset;

    pipe(fd1);
    pipe(fd2);

    number_files = post_files(number_files, argc, argv, parameters_offset);

    if (number_files <= 0) {
        perror("Error. No files are hashable.\n\nExiting program..\n");
        exit(-1);
    }

    sh_mem * shmAddress = (sh_mem *) createSharedMemory();

    int number_slaves = create_slaves(number_files);

    //listen
    int files_processed = 0;
    FILE * hashes = fopen("./Hashes.txt","a"); //"a" for appending at the end of file
    char str[2000];

    //close(fd2[WRITE]);

    //PipeWrite(fd2[WRITE], "HOLA");

    while (files_processed < number_files) {

        //puts("hola jorge\n");
        
        PipeRead(fd2[READ], str);

	//printf("Alguna boludez\n");
        
        // write hash into hashes file
	    fprintf(hashes, "%s\n", str);
	    printf("%s\n", str);

        //enviar msg.text a la view

	    files_processed++;
    }

    fclose(hashes);
    printf("Hashes written to \'./Hashes.txt\'\n");
    //Avisar a la view que terminamos

}

int post_files(int number_files, int argc, const char ** argv, int parameters_offset) {
    int files_posted = 0;
    char buff[500];

    for (int i = parameters_offset; i < argc; i++) {
        if (is_reg_file(argv[i])) {
	    PipeWrite(fd1[WRITE], argv[i]);
            files_posted++;
        } else {
            printf("\'%s\' is not a regular file. It was ignored.\n\n", argv[i]);
        }
    }
    return files_posted;
}

int is_reg_file(const char * path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int create_slaves(int number_files) {
    int number_slaves = slave_number_calc(number_files);

    char * dummyArgs[] = {NULL};
    pid_t pid;

    for (int i = 0; i < number_slaves; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Error creating child process");
            //kill_slaves();
            exit(1);
        }

        if (pid == 0)  {
	    dup2(fd1[READ], 0);
	    dup2(fd2[WRITE], 1);
	    close(fd1[WRITE]);
	    close(fd2[READ]);
            execv("./slave", dummyArgs);
        }
    }

    return number_slaves;
}

int slave_number_calc(int number_files) {
    int files_slaves_ratio = 10;
    int limit = 10;
    int div = number_files / files_slaves_ratio + 1;

    if (div <= limit)
        return div;
    else
        return limit;
}

char * createSharedMemory() {
    int shmId;
    char * shmAddress;

    if ((shmId = shmget(shm_key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("Failed to create shared memory.\n");
        exit(-1);
    }

    if ((shmAddress = shmat(shmId, NULL, 0)) == (char *) -1) {
        perror("Failed to attach segment to data space.\n");
        exit(-1);
    }

    return shmAddress;
}
