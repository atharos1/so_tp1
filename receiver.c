#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG_MAX_LENGHT 128

void die(char * message);
int queue_get(key_t key);

struct message {
    long type;
    char text[MSG_MAX_LENGHT];
};

int main() {
	struct message msg;

	key_t key = 1234;
	int queue_id = queue_get(key);

	int flag = 0;
	while(flag == 0) {
		if (msgrcv(queue_id, &msg, MSG_MAX_LENGHT, 0, 0) < 0)
      		die("Error leyendo el mensaje");

      	if(msg.type == 2) { //Salir
      		flag = 1;
      	} else {
      		printf("%s\n", msg.text);
      	}
      		
	}

}

void die(char * message) {
	perror(message);
	exit(1);
}

int queue_get(key_t key) {
	int id;
	if ((id = msgget(key, 0666 )) < 0)
      die("Error abriendo la cola de mensajes.");

  	return id;
}