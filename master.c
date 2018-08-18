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
    } else if (strcmp(argv[1], "-t") == 0) { // test mode
        if (argc != 2) {
            perror("Error. Test mode can not receive arguments.\n\nExiting program..\n");
            exit(-1);
        } else {
            printf("\n_____________________________\n\n.....Entering test mode.....\n\n_____________________________\n\n");
            run(argc, argv, TEST);
        }
    } else { // normal execution
        printf("\nHashing starting..\n\n");
        run(argc, argv, NORMAL);
    }
}

void run(int argc, char ** argv, int mode) {

    if (mode == TEST) {
        run_test_mode();
        exit(1);
    }

    int parameters_offset = 1;
    int number_files = argc - parameters_offset;

    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror("shm_open()");
    }
    ftruncate(fd, SIZE);
    sh_mem *shm = (sh_mem *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    shm->status = 1;
    shm->currReadLine = 0;
    shm->currWriteLine = 0;

    sem_t * sem = sem_open (SEM_NAME, O_CREAT | O_EXCL, 0644, 0); 
    if(sem == SEM_FAILED) {
        perror("Error. Semaphore could not be initialized.");
        shm->status = 0; //No usemos la memoria compartida, no puedo sincronizar
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
    char str[2000];

    while (files_processed < number_files) {        
        PipeRead(fd2[READ], str);        
	    fprintf(hashes, "%s\n", str);

        strcpy(shm->str[shm->currWriteLine], str);
        shm->currWriteLine++;
        sem_post(sem); //Incremento el semáforo

	    //printf("%s\n", str);
	    files_processed++;
    }

    shm->status = 2;

    fclose(hashes);
    printf("Hashes written to \'./Hashes.txt\'\n");

    //Avisar a la view que terminamos
    int view_connected = (shm->currReadLine > 0);

    munmap(shm, SIZE);
    close(fd);
    sem_unlink(SEM_NAME);

    if(!view_connected)
        shm_unlink(NAME);

}

int post_files(int number_files, int argc, char ** argv, int parameters_offset) {
    int files_posted = 0;

    for (int i = parameters_offset; i < argc; i++) {
        if (valid(argv[i])) {
	    PipeWrite(fd1[WRITE], argv[i]);
            files_posted++;
        } else {
            printf("\'%s\' was ignored.\n\n", argv[i]);
        }
    }
    return files_posted;
}

int valid(char * path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == 0) { //success
        return S_ISREG(path_stat.st_mode);
    } else
        return -1;
}

void create_slaves(int number_files) {
    int number_slaves = slave_number_calc(number_files);

    char * dummyArgs[] = {NULL};
    pid_t pid;

    for (int i = 0; i < number_slaves; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Error creating child process");
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
    int files_slaves_ratio = 10;
    int limit = 10;
    int div = number_files / files_slaves_ratio + 1;

    if (div <= limit)
        return div;
    else
        return limit;
}

void run_test_mode() {
    
}
