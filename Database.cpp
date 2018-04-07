#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/msg.h>
#include <pthread.h>

#define MAX_DATABASE 999

struct msg_st
{
	long int msg_type;
	char text[BUFSIZ];
};

int accountDB [MAX_DATABASE] = {00001, 00011, 00117};
int pinDB [MAX_DATABASE] = {107, 323, 259};
double fundDB [MAX_DATABASE] = {3443.22, 10089.97, 112.00};
int blockDB [MAX_DATABASE];
pthread_mutex_t lock;
pthread_cond_t condition;

void* receiveEdit(void *arg)
{
	int running = 1;
	int msgid = -1;
	struct msg_st data;
	long int msgtype = 20;
	char accountChar[5];
	char pinChar[3];
	char fundChar[50];
	
	msgid = msgget((key_t)1234, 0600 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	while(running){
		pthread_mutex_trylock(&lock);
		if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) == -1){
			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
			exit(EXIT_FAILURE);
		}
	
		int size = sizeof(accountDB) / sizeof(accountDB[0]);
	
		accountChar[0] = data.text[0];
		accountChar[1] = data.text[1];
		accountChar[2] = data.text[2];
		accountChar[3] = data.text[3];
		accountChar[4] = data.text[4];
		pinChar[0] = data.text[5];
		pinChar[1] = data.text[6];
		pinChar[2] = data.text[7];
		
		int textSize = sizeof(data.text) / sizeof(data.text[0]);
		for(int i = 8; i< textSize; i++){
			fundChar[i - 8] = data.text[i];
		}
		
		int account = atoi(accountChar);
		int pin = atoi(pinChar);
		double fund = atof(fundChar);
		size = sizeof(accountDB) / sizeof(accountDB[0]);
		printf("Account Count: %d", size);
	}
}

void sendMsg(int i)
{
	struct msg_st msg;
	//char buffer[BUFSIZ];
	int msgid = -1;
	int running = 1;
	msgid = msgget((key_t)1234, IPC_CREAT | 0600);
	
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	msg.msg_type = 11;
	if(i == 1){
		strcpy(msg.text, "OK");
	}
	else if(i == 2){
		strcpy(msg.text, "NOT OK");
	}
	else if(i == 3){
		strcpy(msg.text, "BLOCK");
	}
	else if(i == 4){
		strcpy(msg.text, "SUCCESS");
	}
	else if(i == 5){
		strcpy(msg.text, "FAILED");
	}
	else{
		char output[50];
		snprintf(output, 50, "%f", fundDB[i - 1000]);
		strcpy(msg.text, output);
	}
	if(msgsnd(msgid, (void*)&msg, BUFSIZ, 0) == -1)
	{
		fprintf(stderr, "msgsnd failed\n");
		exit(EXIT_FAILURE);
	}
}

int calculate(char fund[], int i)
{
	double request = atof(fund);
	int remain;
	if(fundDB[i] >= request){
		fundDB[i] = fundDB[i] - request;
		remain = 2;
	}
	else{
		remain = 3;
	}
	return remain;

}

int receiveRequest(int i)
{
	int running = 1;
	int msgid = -1;
	struct msg_st data;
	long int msgtype = 12;
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
	
	if(strncmp(data.text, "Fund", 4) == 0){
		printf("Fund Request\n");
		receive = 1;
		running = 0;
	}
	else if(strncmp(data.text, "END", 3) == 0){
		receive = 4;
		running = 0;
	}
	else{
		printf("Withdraw Request\n");
		receive = calculate(data.text, i);
		running = 0;
	}
	}
	return receive;
}



void* receivePin(void *arg){
	int running = 1;
	int msgid = -1;
	struct msg_st data;
	long int msgtype = 10;
	int count = 0;
	int i, j;
	
	msgid = msgget((key_t)1234, 0600 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	while(running){
		pthread_mutex_trylock(&lock);
		if(msgrcv(msgid, (void*)&data, BUFSIZ, (long)10, 0) == -1){
			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		else{
			char accountChar[5];
			accountChar[0] = data.text[0];
			accountChar[1] = data.text[1];
			accountChar[2] = data.text[2];
			accountChar[3] = data.text[3];
			accountChar[4] = data.text[4];
			char pinChar[3];
			pinChar[0] = data.text[5];
			pinChar[1] = data.text[6];
			pinChar[2] = data.text[7];
			
			int account = atoi(accountChar);
			int pin = atoi(pinChar);
			
			int size = sizeof(accountDB) / sizeof(accountDB[0]);
			
			i=0;
	
			while(i < size && account != accountDB[i]){
				i++;
			}
			
			
			if(i < size){
				if(pin == pinDB[i]){
					printf("correct\n");
					sendMsg(1);
					running = 0;
				}
				else{
					printf("incorrect\n");
					sendMsg(2);
					count++;
					if(count >= 3){
						printf("block\n");
						blockDB[j] = accountDB[i];
						accountDB[i] = 0;
						j++;
						sendMsg(3);
						exit(EXIT_SUCCESS);
						
					}
				}
			}
			else{
				printf("incorrect account\n");
				sendMsg(2);
			}
		}
	}
		
	int receive = 0;
	running = 1;
	while(running){
		receive = receiveRequest(i);
		if(receive == 1){
			sendMsg(i + 1000);
		}
		else if(receive == 2){
			sendMsg(4);
		}
		else if(receive == 3){
			sendMsg(5);
		}
		else if(receive == 4){
			running = 0;
			exit(EXIT_SUCCESS);
		}
	}
	
	
	if(msgctl(msgid, IPC_RMID, 0) == -1){
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, const char* argv[])
{
	pthread_t id1, id2;

	if(pthread_mutex_init(&lock,NULL) != 0)
	{
		printf("\n mutex init failed\n)");
		return 1;
	}

	pthread_create(&id1, NULL, &receivePin, NULL);
	pthread_create(&id2, NULL, &receiveEdit, NULL);

	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_mutex_destroy(&lock);
	
}



