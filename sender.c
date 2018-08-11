#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG_MAX_LENGHT 128

void die(char * message);
int queue_create(key_t key);

struct message {
    long type;
    char text[MSG_MAX_LENGHT];
};

int main() {
	struct message msg;
	int msg_len;

	key_t key = 1234; //usar ftok para generar clave única
	int queue_id = queue_create(key);

	for(int i = 0; i < 10; i++) {
		msg.type = 1;
		msg.text[0] = i + '0';
		msg.text[1] = '\0';
		msg_len = 2;

		if (msgsnd(queue_id, &msg, msg_len, IPC_NOWAIT) < 0)
        	die("Error agregando mensaje a la cola.");
	}

	msg.type = 2;
	msg.text[0] = 0;
	msg_len = 0;

	if (msgsnd(queue_id, &msg, msg_len, IPC_NOWAIT) < 0)
        	die("Error agregando mensaje a la cola.");

    //msgctl(queue_id, IPC_RMID, NULL); //Borramos la cola
}

void die(char * message) {
	perror(message);
	exit(1);
}

int queue_create(key_t key) {
	int id;
	if ((id = msgget(key, IPC_CREAT | 0666 )) < 0)
      die("Error creando la cola de mensajes.");

  	return id;
}