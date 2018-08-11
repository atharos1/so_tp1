#include "master.h"
#include "sysvmq.h"

#define MASTER_QUEUE_ID 1
#define SLAVE_QUEUE_ID 2

int main(int argc, const char ** argv) {
    if (argc <= 1) { //Argument zero is the program name
        printf("Error. Program should receive at least one argument.\n\nExiting program..\n");
        exit(-1);
    } else {
        printf("Hashing starting..\n\n");
        run(argc,argv);
    }
}

void run(int argc, const char ** argv) {
    int parameters_offset = 1;
    int number_files = argc - parameters_offset;

    key_t queue_key = ftok("./master",ID);

    if(queue_create(queue_key) < 0) {
        perror("Couldn't create message queue. Aborting.");
        die(1);
    }

    number_files = post_files(number_files, argc, argv, parameters_offset, queue_key);

    if (number_files <= 0) {
        printf("Error. No files are hashable.\n\nExiting program..\n");
        exit(-1);
    }

    int number_slaves = create_slaves(number_files);

    
    //listen
    int files_processed = 0;
    struct message msg;

    while(files_processed < number_files) {
        if(queue_read(queue_key, &msg, MASTER_QUEUE_ID, 0, 0) < 0) {
            //Error de lectura
        }

        //Recuperar el mensaje y enviarlo a la view

    }

    //Avisar a la view que terminamos

}

int post_files(int number_files, int argc, const char ** argv, int parameters_offset, key_t queue_key) {
    int files_posted = 0;

    for (int i=parameters_offset; i<argc; i++) {
        if (is_reg_file(argv[i])) {
            queue_post(queue_key, argv[i], SLAVE_QUEUE_ID);
            files_posted ++;
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

    char * dummyArgs[] = {""};
    int pid;

    for(int i = 0; i < number_slaves; i++) {
        pid = fork();

        if(pid < 0) {
            perror("Error creating child process");
            kill_slaves();
            exit(1);
        }

        if(pid != 0) {
            execv("slave", dummyArgs);
        }
    }

    return number_slaves;
}

int slave_number_calc(int number_files) {
    int files_slaves_ratio = 10;
    int limit = 10;
    int div = number_files / files_slaves_ratio;

    if(div <= limit)
        return div;
    else
        return limit;
}