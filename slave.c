#include "slave.h"
#include "pipemng.h"
#include "protocol.h"

int main(int argc, const char ** argv) {

    if (strcmp(argv[1], "-t") == 0) { //Test mode
        if (argc != 2) {
            printf("Error. Test mode can not receive arguments.\n\nExiting program..\n");
            exit(EXIT_FAILURE);
        } else {
            run_test_mode();
            exit(1);
        }
    }

    sem_t * slave_sem = sem_open(SLAVE_SEM_NAME, O_RDWR); //Semaphore to avoid simultaneous access to output pipe
    if (slave_sem == SEM_FAILED) {
        printf("Error opening semaphore.\n\nExiting program..\n");
        exit(EXIT_FAILURE);
    }

    char path[FILE_MAX_LENGTH];
    char md5[MD5_LENGTH+1];

    char output[FILE_MAX_LENGTH + MD5_LENGTH + 3];

    while (pipe_read_onebyone(0, path, slave_sem) > 0) { //Process files until there is no more left in input pipe
        calculate_MD5(path, md5);

        sprintf(output, "%s: %s", path, md5); //Output has format <file>: <hash>

        pipe_write_onebyone(1, output, slave_sem);
    }
    exit(0);
}

void calculate_MD5(const char * path, char * md5) { //Calculate MD5 using md5sum

    char command[strlen("md5sum \"%s\"") + strlen(path)];
    sprintf(command,"md5sum \"%s\"",path);

    FILE * fp = popen(command,"r");
    if (fp == NULL) {
        printf("Error. Could not create file descriptor.\n\nExiting program..\n");
        pclose(fp);
        exit(EXIT_FAILURE);
    }

    fgets(md5, MD5_LENGTH, fp);
    md5[MD5_LENGTH] = 0;

    pclose(fp);
}

void run_test_mode() {
    slave_test_run();
}
