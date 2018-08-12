#include "sysvmq.h"

int queue_post(int queue_id, char * message, int type) {
    int len = strlen(message)+1;
    struct message msg;
    msg.type = type;
    strcpy(msg.text, message);
    return msgsnd(queue_id, &msg, len, IPC_NOWAIT);
}

int queue_read(int queue_id, struct message * msg, int type, int nowait, int incremental) {
    int flags = 0;
    if( nowait == 1 )
        flags = flags | IPC_NOWAIT;
    //if( incremental == 1 )
    //    flags = flags | MSG_EXCEPT;

    return msgrcv(queue_id, msg, MSG_MAX_LENGHT, type, flags);
}

int queue_peek(int queue_id, struct message * msg, int type, int incremental) {
    //int flags = IPC_NOWAIT | MSG_COPY;
    //if( incremental == 1 )
    //    flags = flags | MSG_EXCEPT;

    //return msgrcv(queue_id, msg, MSG_MAX_LENGHT, type, flags);
    return 0;
}

int queue_create(key_t key) {
	return msgget(key, IPC_CREAT | 0666 );
}

int queue_get(key_t key) {
	return msgget(key, 0666 );
}

//int queue_is_empty(key_t key) {
 //??
//}
