#include "master.h"
#include "pipemng.h"
#include "protocol.h"
#include <semaphore.h>

//int queue_id;
int fd1[2];
int fd2[2];

int main(int argc, char ** argv) {
    if (argc <= 1) { //Argument zero is the program name
        perror("Error. Program should receive at least one argument.\n\nExiting program..\n");
        exit(-1);
    } else {
        printf("\nHashing starting..\n\n");
        run(argc,argv);
    }
}

void run(int argc, char ** argv) {
    int parameters_offset = 1;
    int number_files = argc - parameters_offset;

    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror("shm_open()");
    }
    ftruncate(fd, SHM_SIZE);
    sh_mem *shm = (sh_mem *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    shm->status = AWAITING_CONNECTION;
    shm->currReadLine = 0;
    shm->currWriteLine = 0;

    sem_t * sem = sem_open (SEM_NAME, O_CREAT | O_EXCL, 0644, 0); 
    if(sem == SEM_FAILED) {
        perror("Semaforo no se puede inicializar");
        shm->status = ERROR; //No usemos la memoria compartida, no puedo sincronizar
    }

    sleep(3); //Le damos tiempo a la vista para que se conecte

    pipe(fd1);
    pipe(fd2);

    number_files = post_files(number_files, argc, argv, parameters_offset);

    if (number_files <= 0) {
        perror("Error. No files are hashable.\n\nExiting program..\n");
        exit(-1);
    }

    create_slaves(number_files);

    //listen
    int files_processed = 0;
    FILE * hashes = fopen("./Hashes.txt","a"); //"a" for appending at the end of file
    char str[FILE_MAX_LENGTH + MD5_LENGTH + 1];

    while (files_processed < number_files) {

        pipe_read(fd2[READ], str);
	    fprintf(hashes, "%s\n", str);

        if(shm->status != ERROR) {
            strcpy(shm->str[shm->currWriteLine], str);
            shm->currWriteLine++;
            sem_post(sem); //Incremento el semáforo
        }
        
	    files_processed++;
    }

    shm->status = FINISHED;

    fclose(hashes);
    printf("Hashes written to \'./Hashes.txt\'\n");

    close(fd);

    if(shm->status != CONNECTED) {
        munmap(shm, SHM_SIZE);
        shm_unlink(NAME);
        sem_unlink(SEM_NAME);
        sem_close(sem);
    }
        

}

int post_files(int number_files, int argc, char ** argv, int parameters_offset) {
    int files_posted = 0;

    for (int i = parameters_offset; i < argc; i++) {
        if (is_reg_file(argv[i])) {
	    pipe_write(fd1[WRITE], argv[i]);
            files_posted++;
        } else {
            printf("\'%s\' is not a regular file. It was ignored.\n\n", argv[i]);
        }
    }
    return files_posted;
}

int is_reg_file(char * path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void create_slaves(int number_files) {
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
}

int slave_number_calc(int number_files) {
    int div = number_files / FILES_SLAVES_RATIO + 1;

    if (div <= SLAVE_LIMIT)
        return div;
    else
        return SLAVE_LIMIT;
}
