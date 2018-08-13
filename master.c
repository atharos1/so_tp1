#include "master.h"
#include "sysvmq.h"

int queue_id;

int main(int argc, const char ** argv) {
    if (argc <= 1) { //Argument zero is the program name
        perror("Error. Program should receive at least one argument.\n\nExiting program..\n");
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

    if (queue_id = queue_create(queue_key) < 0) {
        perror("Error. Could not create message queue.\n\nExiting program..\n");
        exit(-1);
    }

    number_files = post_files(number_files, argc, argv, parameters_offset);

    if (number_files <= 0) {
        perror("Error. No files are hashable.\n\nExiting program..\n");
        exit(-1);
    }

    int number_slaves = create_slaves(number_files);

    //listen
    int files_processed = 0;
    struct message msg;

    while (files_processed < number_files) {

        if (queue_read(queue_id, &msg, MASTER_QUEUE_ID, 0, 0) < 0) {
            //Error de lectura
        }

	printf("%s\n", msg.text);

        //Recuperar el mensaje y enviarlo a la view
	files_processed++;
    }

    //Avisar a la view que terminamos

}

int post_files(int number_files, int argc, const char ** argv, int parameters_offset) {
    int files_posted = 0;

    for (int i = parameters_offset; i < argc; i++) {
        if (is_reg_file(argv[i])) {
            queue_post(queue_id, argv[i], SLAVE_QUEUE_ID);
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
        printf("Cree proceso %d\n", pid);

        if (pid < 0) {
            perror("Error creating child process");
            //kill_slaves();
            exit(1);
        }

        if (pid == 0) {
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
