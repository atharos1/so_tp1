#include "master.h"
#include "pipemng.h"
#include "protocol.h"

//TODO: NO TIENEN PORQUE SER GLOBALES
int fd1[2];
int fd2[2];

int main(int argc, char ** argv) {
    if (argc <= 1) { //Argument zero is the program name
        printf("Error. Program should receive at least one argument.\n\nExiting program..\n");
        exit(EXIT_FAILURE);
    } else {
        printf("\nHashing starting..\n\n");
        run(argc,argv);
    }
}

void run(int argc, char ** argv) {
    int parameters_offset = 1;
    int number_files = argc - parameters_offset;

    pipe(fd1);
    pipe(fd2);

    number_files = post_files(number_files, argc, argv, parameters_offset);

    close(fd1[WRITE]); //So that slaves read EOF when empty pipe

    if (number_files <= 0) {
        printf("Error. No files are hashable.\n\nExiting program..\n");
        exit(EXIT_FAILURE);
    } else if (number_files > MAX_FILES) {
        printf("Error. Program has a limit of %d files to process.\n\nExiting program..\n",MAX_FILES);
        exit(EXIT_FAILURE);
    }

    sem_t * slave_sem = sem_open (SLAVE_SEM_NAME, O_CREAT | O_EXCL, 0644, 1); //Semaphore initialized in 1 so there is unitary access by slaves in writing pipe
    if(slave_sem == SEM_FAILED) {
        printf("Principal semaphore could not be initialized.\n\nExiting program..\n");
        exit(EXIT_FAILURE);
    }

    int fd_shm = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd_shm < 0) {
        printf("shm_open()");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd_shm, SHM_SIZE);
    sh_mem *shm = (sh_mem *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0); //Shared memory initialized
    shm->status = AWAITING_CONNECTION;
    shm->currReadLine = 0;
    shm->currWriteLine = 0;

    sem_t * sem = sem_open (SEM_NAME, O_CREAT | O_EXCL, 0644, 0); 
    if(sem == SEM_FAILED) {
        printf("Semaphore could not be initialized.\n");
        shm->status = ERROR; //Could not synchronize shared memory, so it will not be used
    }

    sleep(2); //Time for view to connect

    create_slaves(number_files);

    //listen
    int files_processed = 0;
    FILE * hashes = fopen("./Hashes.txt","w");
    char str[FILE_MAX_LENGTH + MD5_LENGTH + 1];

    while (files_processed < number_files) { //Process every file previously verified

        pipe_read(fd2[READ], str); //Read output from slave
	    fprintf(hashes, "%s\n", str);

        if(shm->status != ERROR) {
            strcpy(shm->str[shm->currWriteLine], str); //Transfer slave output to shared memory
            shm->currWriteLine++;
            sem_post(sem); //Increment semaphore
        }
        
	    files_processed++;
    }


    shm->status = FINISHED; //Alert view that there will no be more files added to the buffer

    //Free resources if it corresponds
    //TODO: FALTA CERRAR BIEN LOS PIPES
    sem_unlink(SLAVE_SEM_NAME);
    sem_close(slave_sem);

    fclose(hashes);
    printf("Hashes written to \'./Hashes.txt\'\n");

    close(fd_shm);

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
        if (is_valid(argv[i])) { //Verify if file is valid to generate hash
	    pipe_write(fd1[WRITE], argv[i]); //File added to pending queue (pipe)
            files_posted++;
        } else {
            printf("\'%s\' is not a regular file. It was ignored.\n\n", argv[i]);
        }
    }
    return files_posted;
}

int is_valid(char * path) {
    struct stat path_stat;
    stat(path, &path_stat); // Fails if search permission is denied for one of the directories in the path
    return S_ISREG(path_stat.st_mode);
}

void create_slaves(int number_files) {
    int number_slaves = slave_number_calc(number_files);

    char * dummyArgs[] = {NULL}; //Mandatory for execv
    pid_t pid;

    for (int i = 0; i < number_slaves; i++) {
        pid = fork();

        if (pid < 0) {
            printf("Error creating child process.\n\nExiting program..\n");
            //TODO: kill_slaves();
            exit(EXIT_FAILURE);
        }

        if (pid == 0)  {
            dup2(fd1[READ], 0); //Redirect output and input of slave to pipes
            dup2(fd2[WRITE], 1);
            close(fd1[WRITE]); //Close unnecessary ends of pipe
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
