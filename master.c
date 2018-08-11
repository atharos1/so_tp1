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

    create_slaves(number_files);

    // hacer algo mientras le vayan llegando los hashes y se los pase a la vista
}

void create_queue(int number_files, key_t queue_key) {

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

void create_slaves(int number_files) {
    int number_slaves = slave_number_calc(number_files);

    // crear los slaves
}

int slave_number_calc(int number_files) {

    // algun calculo inventado dependiendo de la cantidad de files
}