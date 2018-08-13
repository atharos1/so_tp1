#include "slave.h"
#include "sysvmq.h"

int queue_id;

int main(int argc, const char ** argv) {

    key_t queue_key = ftok("./master",ID);
    if (queue_id = queue_get(queue_key) < 0 ) {
        perror("Could not get message queue.\n\nExiting program..\n");
        exit(-1);
    }

    struct message msg;
    char buffer[BUFFER_SIZE];

    while (queue_read(queue_id, &msg, SLAVE_QUEUE_ID, 1, 0) >= 0) { //If there are no more messages of type file, queue_read returns -1

        if (msg.text == NULL) {
            printf("Null message\n");
        } else {
            calculate_MD5(msg.text, buffer);
            printf("termino de calcular\n");
            queue_post(queue_id, buffer, MASTER_QUEUE_ID);
        }
    }
    exit(0);
}

int calculate_MD5(const char * path, char * buffer) {

    char command[sizeof("md5sum %s") + sizeof(path)];
    sprintf(command,"md5sum %s",path);

    FILE * fp = popen(command,"r");
    if (fp == NULL) {
        perror("Error. Could not create file descriptor.\n\nExiting program..\n");
        pclose(fp);
        exit(-1);
    }
    //Path is copied into buffer to return "<filename>: <md5 hash>" to master
    for (int i = 0; path[i] != '\0'; i++) {
        *(buffer++) = path[i];
    }

    *(buffer++) = ':';
    *(buffer++) = ' ';

    char c;
    for (int i = 0; i < MD5_LENGTH; i++) {
        c = fgetc(fp);

        if (!isxdigit(c)) {
            perror("Error. Non hexadecimal digit in hash.\n\nExiting program..\n");
            pclose(fp);
            exit(-1);
        }

        *(buffer++) = c;
    }

    *buffer = '\0';

    pclose(fp);
    return 0;
}
