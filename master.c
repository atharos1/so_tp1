#include "master.h"
#include "pipemng.h"
#include "protocol.h"

//TODO: NO TIENEN PORQUE SER GLOBALES
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

    sem_t * slave_sem = sem_open (SLAVE_SEM_NAME, O_CREAT | O_EXCL, 0644, 1); //Inicializamos semaforo en 1, para garantizar acceso unitario al pipe de escritura los esclavos
    if(slave_sem == SEM_FAILED) {
        perror("Semaforo principal no se puede inicializar");
        exit(1);
    }

    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror("shm_open()");
        exit(-1);
    }
    ftruncate(fd, SHM_SIZE);
    sh_mem *shm = (sh_mem *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Inicializamos la memoria compartida
    shm->status = AWAITING_CONNECTION;
    shm->currReadLine = 0;
    shm->currWriteLine = 0;

    sem_t * sem = sem_open (SEM_NAME, O_CREAT | O_EXCL, 0644, 0); 
    if(sem == SEM_FAILED) {
        perror("Semaforo no se puede inicializar");
        shm->status = ERROR; //No usamos la memoria compartida, no puedo sincronizar
    }

    sleep(3); //Le damos tiempo a la vista para que se conecte

    pipe(fd1);
    pipe(fd2);

    number_files = post_files(number_files, argc, argv, parameters_offset); //TODO: ABORTAR SI SUPERA MAX_FILES

    close(fd1[WRITE]); //Garantizamos que esclavos lean EOF cuando el pipe quede vacío

    if (number_files <= 0) {
        perror("Error. No files are hashable.\n\nExiting program..\n");
        exit(-1);
    }

    create_slaves(number_files);

    //listen
    int files_processed = 0;
    FILE * hashes = fopen("./Hashes.txt","a"); //"a" for appending at the end of file
    char str[FILE_MAX_LENGTH + MD5_LENGTH + 1];

    while (files_processed < number_files) { //Proceso todos los archivos verificados previamente

        pipe_read(fd2[READ], str); //Leo salida del esclavo
	    fprintf(hashes, "%s\n", str);

        if(shm->status != ERROR) {
            strcpy(shm->str[shm->currWriteLine], str); //Copio salida del esclavo a memoria compartida
            shm->currWriteLine++;
            sem_post(sem); //Incremento el semáforo
        }
        
	    files_processed++;
    }


    shm->status = FINISHED; //Indicamos a la vista que no se agregarán más archivos al buffer

    //Liberamos recursos si nos corresponde
    //TODO: FALTA CERRAR BIEN LOS PIPES
    sem_unlink(SLAVE_SEM_NAME);
    sem_close(slave_sem);

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

int post_files(int number_files, int argc, char ** argv, int parameters_offset) { //Verificamos que el archivo sea válido para generar hash y lo agregamos a la cola de pendientes (pipe)
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

    char * dummyArgs[] = {NULL}; //Obligatorio para execv
    pid_t pid;

    for (int i = 0; i < number_slaves; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Error creating child process");
            //TODO: kill_slaves();
            exit(1);
        }

        if (pid == 0)  { //Redireccionamos salida y entrada del hijo a los pipes y cargamos el esclavo
            dup2(fd1[READ], 0);
            dup2(fd2[WRITE], 1);
            close(fd1[WRITE]); //Cerramos extremos de pipe innecesarios
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
