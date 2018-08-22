#include "master.h"
#include "pipemng.h"
#include "protocol.h"

int master_fd[2];
int slave_fd[2];
int slavesPID[SLAVE_LIMIT];

int main(int argc, char ** argv) {
    if (argc <= 1) { //Argument zero is the program name
        printf("Error. Program should receive at least one argument.\n\nExiting program..\n");
        exit(EXIT_FAILURE);
    } else if (strcmp(argv[1], "-t") == 0) { //Test mode
        if (argc != 2) {
            printf("Error. Test mode can not receive arguments.\n\nExiting program..\n");
            exit(EXIT_FAILURE);
        } else {
            printf("\n_____________________________\n\n.....Entering test mode.....\n\n_____________________________\n\n");
            run(argc, argv, TEST);
        }
    } else { //Normal execution
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

    pipe(master_fd);
    pipe(slave_fd);

    number_files = post_files(number_files, argc, argv, parameters_offset);

    close(master_fd[WRITE]); //So that slaves read EOF when empty pipe

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

    int number_slaves = create_slaves(number_files);

    //listen
    int files_processed = 0;
    FILE * hashes = fopen("./Hashes.txt","w");
    char str[FILE_MAX_LENGTH + MD5_LENGTH + 1];

    while (files_processed < number_files) { //Process every file previously verified

        pipe_read(slave_fd[READ], str); //Read output from slave
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
    close(master_fd[READ]);
    close(slave_fd[READ]);
    close(slave_fd[WRITE]);

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

    int ret;
    for (int i = 0; i < number_slaves; i++) {
        waitpid(slavesPID[i], &ret, 0);
    }
}

int post_files(int number_files, int argc, char ** argv, int parameters_offset) {
    int files_posted = 0;

    for (int i = parameters_offset; i < argc; i++) {
        if (is_valid(argv[i])) { //Verify if file is valid to generate hash
	    pipe_write(master_fd[WRITE], argv[i]); //File added to pending queue (pipe)
            files_posted++;
        } else {
            printf("\'%s\' was ignored.\n\n", argv[i]);
        }
    }
    return files_posted;
}

int is_valid(char * path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == -1 ) // Fails if search permission is denied for one of the directories in the path
        return -1;
    return S_ISREG(path_stat.st_mode);
}

int create_slaves(int number_files) {
    int number_slaves = slave_number_calc(number_files);

    char * args[] = {NULL};
    pid_t pid;

    for (int i = 0; i < number_slaves; i++) {
        pid = fork();

        if (pid < 0) {
            printf("Error creating child process.\n\nExiting program..\n");
            kill_slaves(i);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)  {
            dup2(master_fd[READ], 0); //Redirect output and input of slave to pipes
            dup2(slave_fd[WRITE], 1);
            close(master_fd[WRITE]); //Close unnecessary ends of pipe
            close(slave_fd[READ]);
            execv("./Binaries/slave", args);
        }

        slavesPID[i] = pid;
    }

    return number_slaves;
}

int slave_number_calc(int number_files) {
    int div = number_files / FILES_SLAVES_RATIO + 1;

    if (div <= SLAVE_LIMIT)
        return div;
    else
        return SLAVE_LIMIT;
}

void kill_slaves(int n) {
    for(int i = 0; i < n; i++) {
        kill(slavesPID[i], 9);
    }
}

void run_test_mode() {
    master_test_run();
    printf("cool");
    char * args[] = {"./Binaries/slave", "-t"};
    execv("./Binaries/slave", args);
}
