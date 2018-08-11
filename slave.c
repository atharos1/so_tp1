#include "slave.h"
#include "sysvmq.h"

int queue_id;

int main(int argc, const char ** argv) {

    key_t queue_key = ftok("./master",ID);
    if (queue_id = queue_get(key) < 0 ) {
        perror("Could not get message queue.\n\nExiting program..\n");
        exit(-1);
    }

    struct message msg;
    char buffer[MSG_MAX_LENGHT + MD5_MAX_LENGHT];

    while (!queue_is_empty(queue_id)) {

        //clear text in message struct
        for (int i = 0; i < MSG_MAX_LENGHT; i++) {
            msg.text[i] = 0;
        }

        queue_read(queue_id, &msg, SLAVE_QUEUE_ID, 0, 0);
        if (msg.text == NULL) {
            printf("Null message\n");
        } else {
            calculate_MD5(msg.text, buffer);
            queue_post(queue_id, buffer, MASTER_QUEUE_ID);
        }
    }

    exit(0);
}

int calculate_MD5(const char * path, char * buffer) {
  // usamos una funcion de c o md5sum?
}