#include "slave.h"
#include "sysvmq.h"

int main(int argc, const char ** argv) {

    key_t queue_key = ftok("./master",ID);

    struct message msg;
    char buffer[MSG_MAX_LENGHT + MD5_MAX_LENGHT];

    while (!queue_is_empty(queue_key)) {

        //clear text in message struct
        for (int i = 0; i < MSG_MAX_LENGHT; i++) {
            msg.text[i] = 0;
        }

        queue_read(queue_key, &msg, SLAVE_QUEUE_ID, 0, 0);
        if (msg.text == NULL) {
            printf("Null message\n");
        } else {
            calculate_MD5(msg.text, buffer);
            queue_post(queue_key, buffer, MASTER_QUEUE_ID);
        }
    }

    exit(0);
}

int calculate_MD5(const char * path, char * buffer) {
  // usamos una funcion de c o md5sum?
}