//////////////////////////////////////////////////////////////////////////////////
// Course: SYSC4001
// Engineer: qiyang xia
// Engineer id:100837661
// Create Date:    2016/12/12
// Design Name:    
// Module Name:    CDBoost
// Project Name:   teamproject
// Target Devices: 
// Tool versions:   linux,DEV
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef PCB_H
#define PCB_H

#include<iostream>
#include<malloc.h>
#include<stdlib.h>
#include<cstring>
#include <stdio.h> 
#include<fstream>
#include<string>
#include <sstream>

using namespace std;

#define OK 1 
#define EVERFLOW -1 
#define PCBSIZE 20 

//#define NULL 0 
#define READY 1
#define RUN 2
#define WAIT 3


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



void read();
void inunization();

void InitPcb(PCBList &nullPcb);

void InsertReadyPcb(PCBList &readyPcb,PCBList &pcb);
int Delete(int id,PCBList &readyPcb,PCBList &nullPcb);
void PrintPCB(PCBList &readyPcb);
void sjfInsertReadyPcb(PCBList &readyPcb,PCBList &pcb);
void PriorityfInsertReadyPcb(PCBList &readyPcb,PCBList &pcb);
void rrInsertReadyPcb(PCBList &readyPcb,PCBList &pcb);

#endif
