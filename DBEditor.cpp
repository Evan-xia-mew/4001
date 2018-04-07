#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include<pthread.h>

struct msg_st{
	long int msg_type;
	char text[BUFSIZ];
};

int main(){
	int running = -1;
	struct msg_st msg;
	char buffer[BUFSIZ];
	int msgid = -1;
	
	msgid = msgget((key_t)1234, IPC_CREAT | 0600);
	
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	while(running){
		printf("Enter Account Number: ");
		fgets(buffer, BUFSIZ, stdin);
		printf("Enter PIN: ");
		fgets(buffer + 5, BUFSIZ, stdin);
		printf("Enter Fund: ");
		fgets(buffer + 8, BUFSIZ, stdin);
		msg.msg_type = 20;
		strcpy(msg.text, buffer);
		if(msgsnd(msgid, (void*)&msg, BUFSIZ, 0) == -1)
		{
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		if(strncmp(buffer, "END", 3) == 0){
		running = 0;
		}
	}
	exit(EXIT_SUCCESS);
}
