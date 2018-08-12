#ifndef _SYSVMQ_H
#define _SYSVMQ_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_MAX_LENGHT 128

struct message {
    long type;
    char text[MSG_MAX_LENGHT];
};

int queue_post(int queue_id, char * message, int type);
int queue_read(int queue_id, struct message * msg, int type, int nowait, int incremental);
int queue_peek(int queue_id, struct message * msg, int type, int incremental);
int queue_create(key_t key);
int queue_get(key_t key);
int queue_is_empty(key_t key);

#endif
