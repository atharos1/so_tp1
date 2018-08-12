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
    char buffer[MSG_MAX_LENGHT + MD5_MAX_LENGHT];

    for (int i = 0; i < MSG_MAX_LENGHT; i++) {
        msg.text[i] = 0;
    }

    while (queue_read(queue_id, &msg, SLAVE_QUEUE_ID, 1, 0) >= 0) {

        if (msg.text == NULL) {
            printf("Null message\n");
        } else {
            calculate_MD5(msg.text, buffer);
            queue_post(queue_id, buffer, MASTER_QUEUE_ID);
        }

        //clear text in message struct
        for (int i = 0; i < MSG_MAX_LENGHT; i++) {
            msg.text[i] = 0;
        }
    }

    exit(0);
}

int calculate_MD5(const char * path, char * buffer) {
  // usamos una funcion de c o md5sum?
  buffer[0] = 'M';
  buffer[1] = 'D';
  buffer[2] = '5';
  buffer[3] = 0;
}
