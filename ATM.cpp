#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include<pthread.h>

#define MAX_ACCOUNT 5
#define MAX_PIN 3

struct pin_msg{
	long int msg_type;
	char text[MAX_ACCOUNT + MAX_PIN];
};

struct fund_msg{
	long int msg_type;
	char text[1000];
};

struct msg_st{
	long int msg_type;
	char text[BUFSIZ];
};

int receiveMsg(void)
{
	int running = 1;
	int msgid = -1;
	struct msg_st data;
	long int msgtype = 11;
	int receive;
	
	msgid = msgget((key_t)1234, 0600 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	while(running){
		if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) == -1){
			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
			exit(EXIT_FAILURE);
		}
	
		if(strncmp(data.text, "OK", 2) == 0){
			receive = 1;
			running = 0;
		}
		else if(strncmp(data.text, "NOT OK", 6) == 0){
			receive = 2;
			running = 0;
		}
		else if(strncmp(data.text, "BLOCK", 5) == 0){
			receive = 3;
			running = 0;
		}
		else if(strncmp(data.text, "SUCCESS", 7) == 0){
			receive = 4;
			running = 0;
		}
		else if(strncmp(data.text, "FAILED", 6) == 0){
			receive = 5;
			running = 0;
		}
		else{
			receive = 6;
			printf("%s\n", data.text);
			running = 0;
		}
	}
	return receive;
}

int main(){
	int running = -1;
	struct pin_msg pinmsg;
	char buffer[BUFSIZ];
	int msgid = -1;
	int receive = 2;
	
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
		
		pinmsg.msg_type = 10;
		strcpy(pinmsg.text, buffer);
		if(msgsnd(msgid, (void*)&pinmsg, MAX_ACCOUNT + MAX_PIN, 0) == -1)
		{
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		receive = receiveMsg();
		if(receive != 2){
			running = 0;
			break;
		}
		printf("NOT OK\n");
	}
	
	if(receive == 1){
		printf("OK\n");
		
		char request[BUFSIZ];
		struct fund_msg fundmsg;
		running = -1;
		
		while(running){
		printf("Enter Request (Fund/Withdraw/END): ");
		fgets(request, BUFSIZ, stdin);
		fundmsg.msg_type = 12;
		if(strncmp(request, "Fund", 4) == 0){
			strcpy(fundmsg.text, request);
			if(msgsnd(msgid, (void*)&fundmsg, BUFSIZ, 0) == -1)
			{
				fprintf(stderr, "msgsnd failed\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(strncmp(request, "Withdraw", 8) == 0){
			printf("Enter Amount: ");
			fgets(request, BUFSIZ, stdin);
			strcpy(fundmsg.text, request);
			if(msgsnd(msgid, (void*)&fundmsg, BUFSIZ, 0) == -1)
			{
				fprintf(stderr, "msgsnd failed\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(strncmp(request, "END", 3) == 0){
			strcpy(fundmsg.text, request);
			if(msgsnd(msgid, (void*)&fundmsg, BUFSIZ, 0) == -1)
			{
				fprintf(stderr, "msgsnd failed\n");
				exit(EXIT_FAILURE);
			}
			running = 0;
			exit(EXIT_SUCCESS);
		}
		
		receive = receiveMsg();
		if(receive == 4){
			printf("SUCCESS\n");
		}
		else if(receive == 5){
			printf("FAILED\n");
		}
	}
	}
	else if(receive == 3){
		printf("BLOCK\n");
		exit(EXIT_SUCCESS);
	}
	
	
	exit(EXIT_SUCCESS);
}
