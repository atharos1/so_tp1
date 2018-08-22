#include "slave.h"
#include "pipemng.h"
#include "protocol.h"

int main(int argc, const char ** argv) {

    sem_t * slave_sem = sem_open(SLAVE_SEM_NAME, O_RDWR);
    if (slave_sem == SEM_FAILED) {
        perror("Error opening semaphore");
        exit(EXIT_FAILURE);
    }

    char path[FILE_MAX_LENGTH];
    char md5[MD5_LENGTH+1];

    char output[FILE_MAX_LENGTH + MD5_LENGTH + 1];

    //sem_post(slave_sem);

    while (pipe_read_onebyone(0, path, slave_sem) > 0) {
        //printf("\n\n%s\n\n", path);
        calculate_MD5(path, md5);

        sprintf(output, "%s: %s", path, md5);

        pipe_write_onebyone(1, output, slave_sem);

    }

    exit(0);
}

void calculate_MD5(const char * path, char * md5) {

    char command[strlen("md5sum \"%s\"") + strlen(path)];
    sprintf(command,"md5sum \"%s\"",path);

    FILE * fp = popen(command,"r");
    if (fp == NULL) {
        perror("Error. Could not create file descriptor.\n\nExiting program..\n");
        pclose(fp);
        exit(-1);
    }

    fgets(md5, MD5_LENGTH, fp);
    md5[MD5_LENGTH] = 0;

    pclose(fp);
}
