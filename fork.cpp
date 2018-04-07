//////////////////////////////////////////////////////////////////////////////////
// Course: SYSC4001
// Engineer: Zening Jiang
// Create Date:    2016/12/12
// Design Name:    pcb1 
// Module Name:    CDBoost
// Project Name:   teamproject
// Target Devices: 
// Tool versions:   linux,DEV
//
//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <queue>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include "pcb.h"
#include <fstream>

struct PCB1
{
    int id;
    int pid;
    char state[10];
};

struct msg_st
{
	long int msgtype;
	char text[20];
};

    int main(){  
        int pid[20];
        int state;
        int i, n, count; 
      	struct PCB1 pcb1[20];
      	struct msg_st sndmsg, rcvmsg;
      	int msgid;
      	char str[100];
      	int running = 1;
      	
      	PCBList nullPcb;
	PCBList readyPcb;
	PCBList readyPcbtail;
	PCBList Create(PCBList &nullPcb);
      	

      	msgid = msgget((key_t)1234, IPC_CREAT | 0666);
      	if(msgid == -1){
      		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
      	}
      
      	//for(i = 0; i < 5; i++){
      	//	pcb1[i].id = i;
      	//	pcb1[i].pid = 0;
      	//	strcpy(pcb1[i].state, "ready");
      	//}
      	
      	//for(i = 0; i < 5; i++){
      	//	printf("ID %d is %s\n", pcb1[i].id, pcb1[i].state);
      	//}
      
  	//for(i = 0; i < 5; i++){
  	//	pid[i] = fork();
  	//	if(pid[i]  == 0){
  	//		while(running){
  	//		if(msgrcv(msgid, (void*)&rcvmsg, 100, getpid(), 0) == -1){
	//			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
	//			exit(EXIT_FAILURE);
	//		}
	//		if(strncmp(rcvmsg.text, "Running", 7) == 0){
				//printf("Process %d get receive message %s\n", getpid(), rcvmsg.text);
  	//			//printf("parent pid is %d\n", getppid());
  	//			execlp("./ABP", "ABP", "input_abp_1.txt", NULL);
	//			running = 0;
	//		}
			
  	//		}
  	//		exit(0);
  	//	}
  		
  	//}
  	
	
inunization();

int on=0;


PCBList p;

int deleteId;

InitPcb(nullPcb);

read();

readyPcbtail=(PCB*)malloc(sizeof(PCB));

readyPcb=readyPcbtail;

readyPcbtail->id=0;

readyPcbtail->arrivetime=0;

readyPcbtail->time=0;

readyPcbtail->next=NULL;

n = 0;
count = 0;
do

{

cout<<"******************************"<<endl;;

cout<<"1.create by fcfs"<<endl;//First come first service schedule function

cout<<"2.delete"<<endl;

cout<<"3.print"<<endl;

cout<<"4.exit"<<endl;

cout<<"5.using sjf to create"<<endl;//Shortest job first schedule function

cout<<"6.using Priority to create"<<endl;//Priority schedule function

cout<<"7.using rr to create"<<endl;//Round R schedule function

cout<<"8.start processes"<<endl;

cout<<"******************************"<<endl;

cin>>on;

switch(on)

{

case 1: p=Create(nullPcb); 
	//cout<<"N is "<<n<<endl;
	pid[n] = fork();
	
	if(pid[n] == 0){
		p->name = getpid();
		InsertReadyPcb(readyPcb,p);
		PrintPCB(readyPcb);
		while(running){
			sleep(1);
  			if(msgrcv(msgid, (void*)&rcvmsg, 100, getpid(), 0) == -1){
				fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
				exit(EXIT_FAILURE);
			}
			if(strncmp(rcvmsg.text, "Running", 7) == 0){
				//execv("usr/bin/xterm", NULL);
				cout<<"Process "<<getpid()<<" get receive message "<<rcvmsg.text<<endl;
				//for(i = 0; i<20; i++){
				//	if(getpid() != readyPcb->next->name){
				//		readyPcb->next = readyPcb->next->next;
				//		break;
				//	}
				//	printf("error\n");
				//	exit(EXIT_FAILURE);
				//}
				
				PCBList temp, head;
				head = readyPcb->next;
				char a=' '; 
				while(head){
					if(getpid() == head->name){
						temp = head;	
					}
					head = head->next;
				}
				//cout<<"!!!!!!"<<endl;
				//cout<<temp->arrivetime<<a<<getpid()<<a<<"controlIn"<<a<<temp->time<<endl;
				ofstream inFile;  
				inFile.open("input.txt",ios::out);
				
				inFile<<temp->arrivetime<<a<<getpid()<<a<<"controlIn"<<a<<temp->time; 
				inFile.close();
  				execlp("./ABP", "ABP", "input.txt", NULL);
  				exit(0);
			}
			
  		}
  			
		}
		p->name = pid[n];
		InsertReadyPcb(readyPcb,p);
		n++;
break; 

case 2: cout<<"enter the one need delete"<<endl;cin>>deleteId;Delete(deleteId,readyPcb,nullPcb);break;

case 3: 

PrintPCB(readyPcb);break;

case 4: exit(0);

case 5:  p=Create(nullPcb); sjfInsertReadyPcb(readyPcb,p);break; 

case 6:  p=Create(nullPcb); PriorityfInsertReadyPcb(readyPcb,p);break; 

case 7:  p=Create(nullPcb); rrInsertReadyPcb(readyPcb,p);break; 

case 8: 
	for(i = 0; i < n; i++){
		sleep(2);
  		sndmsg.msgtype = readyPcb->next->name;
  		strcpy(sndmsg.text, "Running");
  		if(msgsnd(msgid, (void*)&sndmsg, 100, 0) == -1){
  			fprintf(stderr, "msgsnd failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
  		}
  		cout<<"send message to "<<readyPcb->next->name<<endl;
  		running = 1;
  		while(running){
  			//if(waitpid(readyPcb->next->name, NULL, 0)){
  			if(waitpid(-1, NULL, 0)){
  				printf("Process %d finished.\n", readyPcb->next->name);
  				//delete first pcb
  				readyPcb->next = readyPcb->next->next;	
  				running = 0;
  			}
  		}
  	}
  	count = 0;
  	n = 0;
  	break;

default:

cout<<"please enter 1 to 4\n";

}
	

}while(on!=4);		
}
