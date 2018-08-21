#include "slave.h"
#include "pipemng.h"

int queue_id;

int main(int argc, const char ** argv) {

    char path[FILE_MAX_LENGTH];
    char md5[MD5_LENGTH+1];

    char output[FILE_MAX_LENGTH + MD5_LENGTH + 1];

    while (PipeRead(0, path) > 0) {
        printf("\n\n%s\n\n", path);
        calculate_MD5(path, md5);

        sprintf(output, "%s:%s", path, md5);

        PipeWrite(1, output);
    }

    exit(0);
}

void calculate_MD5(const char * path, char * md5) {

    char command[strlen("md5sum %s") + strlen(path)];
    sprintf(command,"md5sum %s",path);

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
