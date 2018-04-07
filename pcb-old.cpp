//////////////////////////////////////////////////////////////////////////////////
// Course: SYSC4001
// Engineer: qiyang xia
// Engineer id:100837661
// Create Date:    2016/12/12
// Design Name:    pcb
// Module Name:    CDBoost
// Project Name:   teamproject
// Target Devices: 
// Tool versions:   linux,DEV
//
//////////////////////////////////////////////////////////////////////////////////

#include<iostream>
#include<malloc.h>
#include<stdlib.h>
#include<cstring>
#include <stdio.h>
#include<fstream>
#include<string>

#include <sstream> 
//#define getpch(type)(type*)malloc(sizeof(type)) 
using namespace std;

#define OK 1 
#define EVERFLOW -1 
#define PCBSIZE 20 

#define NULL 0 
#define READY 1
#define RUN 2
#define WAIT 3


int n=0;
char x[10];//runing time
char y[10]; //pid name
char z[10];
char q[10]; //arrive time 
int a,b,c;
int at[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int pidnub[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int rt[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
int temp=0;
int i=0;
string str;



typedef struct PCB 

{
int name;

int id;

int priority; 

int arrivetime; 

int time;

int state;

struct PCB *next; 

}PCB,*PCBList;



/////////////////////////////////////////////////////////////////////////////////////////////
void inunization()                         
{
 
printf("\n\n\t\t**************************************\t\t\n");
printf("\t\t\t\t PCB\n");
printf("\n\n\t\t**************************************\t\t\n");
printf("\n\n\t\tenter any keyborad to start.\t\t\n");


getchar();
 
}

PCB pcb[PCBSIZE];

PCBList nullPcb;

PCBList readyPcb;

PCBList readyPcbtail;

void InitPcb(PCBList &nullPcb);

PCBList Create(PCBList &nullPcb);

void InsertReadyPcb(PCBList &readyPcb,PCBList &pcb);

int Delete(int id,PCBList &readyPcb,PCBList &nullPcb);

void PrintPCB(PCBList &readyPcb);

void sjfInsertReadyPcb(PCBList &readyPcb,PCBList &pcb);

void PriorityfInsertReadyPcb(PCBList &readyPcb,PCBList &pcb);

void rrInsertReadyPcb(PCBList &readyPcb,PCBList &pcb);


/////////////////////////////////////////////////////////////////////////////////////////////

void read()
{

ifstream infile("input_abp_0.txt");

string s;

while(getline(infile,s)){
	n++;
}
 cout<<n<<endl; //running for testing
 
ifstream infile2("input_abp_0.txt");

for(int k=0;k<n;k++){
	
  infile2>>x>>y>>z>>q;
  
  //cout<<" X is "<<x<<" y is "<<y<<" q is "<<q<<endl;//running for testing
  a=atoi(x);
  
  b=atoi(y);
  
  c=atoi(q);
  
  at[k]=a;
  
  pidnub[k]=b;
  
  rt[k]=c;


  //cout<<" X is "<<a<<" y is "<<b<<" q is "<<c<<endl;//running for testing
  
}
for(int k=0;k<n;k++){
	
  cout<<" X is "<<at[k]<<" y is "<<pidnub[k]<<" q is "<<rt[k]<<endl;

}

}


/////////////////////////////////////////////////////////////////////////////////////////////

void InitPcb(PCBList &nullPcb)

{

nullPcb=&pcb[0];

for(int i=0;i<PCBSIZE-1;i++){

pcb[i].id=i;

pcb[i].next=&pcb[i+1];

}

pcb[PCBSIZE-1].next=NULL;

cout<<"strat"<<endl;

}


/////////////////////////////////////////////////////////////////////////////////////////////


PCBList Create(PCBList &nullPcb)
{ 
PCBList pcbP;

if(nullPcb){
pcbP=nullPcb; 

nullPcb=nullPcb->next;

//	cout<<"c is"<<c<<endl;
i++; 
pcbP->id=i;

pcbP->priority=1;

pcbP->name=pidnub[temp];

 //cout<<pcbP->id<<endl;
 
//printf("enter arrivetime :\n");

//scanf("%d",&pcbP->arrivetime);

pcbP->arrivetime=at[temp];

cout<<pcbP->arrivetime<<endl;

//printf("enter running time:\n");

//scanf("%d",&pcbP->time);

pcbP->time=rt[temp];

cout<<pcbP->time<<endl;

pcbP->state=READY;

pcbP->next=NULL;


char t[256];

string s;

sprintf(t, "%d", at[temp]);

s=t;

char t1[256];

string s1;

sprintf(t1, "%d", rt[temp]);

s1=t1;

str.append(s);

str.push_back(' ');

str.append("1");

str.push_back(' ');

str.append("controlIn");

str.push_back(' ');

str.append(s1);

str.push_back('.');

cout<<str;

temp++;

}
 
return pcbP;

}



/////////////////////////////////////////////////////////////////////////////////////////////
int Delete(int id,PCBList &readyPcb,PCBList &nullPcb)
{ 
PCBList pcbT,pcbF;

if(pcbT) {

while(pcbT) {

if(pcbT->id==id) {

pcbF->next=pcbT->next;

pcbT->next=nullPcb;

nullPcb=pcbT;

printf("delete done\n");

return OK;

}

pcbT=pcbT->next;

pcbF=pcbF->next;

}

if(!pcbT) {

printf("no one need delete\n");

} }

else{

printf("no one need delete\n");

}

return OK;

}




/////////////////////////////////////////////////////////////////////////////////////////////
void PrintPCB(PCBList &readyPcb)

{ 
ifstream infile("input_abp_0.txt");

string s;
int t=0;

while(getline(infile,s)){
	
	t++;
}

cout<<"the total number of threads there is :"<<t<<endl;

PCBList pcbP;

printf("queue table:\n");

printf("\t\tid\tname\tarrivetime\trunning_time\tstate\n");

pcbP=readyPcb->next;

while(pcbP)

{
	
printf("\t\t%d\t%d\t%d\t\t%d\t\t%d\n",pcbP->id,pcbP->name,pcbP->arrivetime,pcbP->time,pcb->state);


pcbP=pcbP->next;

}

}




/////////////////////////////////////////////////////////////////////////////////////////////
void InsertReadyPcb(PCBList &readyPcb,PCBList &pcb)

{

PCBList pcbF=readyPcb;

PCBList pcbT=readyPcb->next;

if(pcbT)

{

while(pcbT)

{

if(pcbT->arrivetime>pcb->arrivetime)

{

pcb->next=pcbT;

pcbF->next=pcb;

printf("successful\n");

return;

}

pcbT=pcbT->next;

pcbF=pcbF->next;

}

if(!pcbT)

{

pcbF->next=pcb;

}

}

else{

pcbF->next=pcb;

printf("successful\n");

}

}



/////////////////////////////////////////////////////////////////////////////////////////////
void sjfInsertReadyPcb(PCBList &readyPcb,PCBList &pcb)
{
	
PCBList pcbF=readyPcb;

PCBList pcbT=readyPcb->next;

if(pcbT)

{

while(pcbT)

{

if((pcbT->time)+(pcbT->arrivetime)>(pcb->time)+(pcb->arrivetime))

{

pcb->next=pcbT;

pcbF->next=pcb;

printf("successful\n");

return;

}

pcbT=pcbT->next;

pcbF=pcbF->next;

}

if(!pcbT)

{

pcbF->next=pcb;

}

}

else{

pcbF->next=pcb;

printf("successful\n");

}

}

	

	
/////////////////////////////////////////////////////////////////////////////////////////////	
void PriorityfInsertReadyPcb(PCBList &readyPcb,PCBList &pcb){

PCBList pcbF=readyPcb;

PCBList pcbT=readyPcb->next;

if(pcbT)

{

while(pcbT)

{

if(pcbT->priority>pcb->priority)

{

pcb->next=pcbT;

pcbF->next=pcb;

printf("successful\n");

return;

}

pcbT=pcbT->next;

pcbF=pcbF->next;

}

if(!pcbT)

{

pcbF->next=pcb;

}

}

else{

pcbF->next=pcb;

printf("successful\n");

}

}




/////////////////////////////////////////////////////////////////////////////////////////////
void rrInsertReadyPcb(PCBList &readyPcb,PCBList &pcb)

{
	
cout<<"Please enter RR time:\n";//Request RR time

int rr;

scanf("%d",&rr);

PCBList pcbF=readyPcb;

PCBList pcbT=readyPcb->next;

temp=pcb->time;
	
if(pcbT)

{

while(pcbT)

{

if(pcbT->arrivetime>pcb->arrivetime)

{

if(temp-rr<0)

{

pcb->next=pcbT;
		
pcbF->next=pcb;
	    
}else

{

do

{

temp=temp-rr;
	    		
pcb->next=pcbT;

pcbF->next=pcb;

}while(temp-rr==0);
	    
}

printf("successful\n");

return;

}
	
pcbT=pcbT->next;

pcbF=pcbF->next;

}

if(!pcbT)

{

pcbF->next=pcb;

}

}else

{

pcbF->next=pcb;

printf("successful\n");

}

}







/////////////////////////////////////////////////////////////////////////////////////////////
main()
{
	
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

cout<<"******************************"<<endl;

cin>>on;

switch(on)

{

case 1: p=Create(nullPcb); InsertReadyPcb(readyPcb,p);break; 

case 2: cout<<"enter the one need delete"<<endl;cin>>deleteId;Delete(deleteId,readyPcb,nullPcb);break;

case 3: PrintPCB(readyPcb);break;

case 4: exit(0);

case 5:  p=Create(nullPcb); sjfInsertReadyPcb(readyPcb,p);break; 

case 6:  p=Create(nullPcb); PriorityfInsertReadyPcb(readyPcb,p);break; 

case 7:  p=Create(nullPcb); rrInsertReadyPcb(readyPcb,p);break; 

default:

cout<<"please enter 1 to 4\n";

}

}while(on!=4);

}
