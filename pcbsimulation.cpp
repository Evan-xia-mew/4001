//////////////////////////////////////////////////////////////////////////////////
// Course: SYSC4001
// Engineer: qiyang xia
// Engineer id:100837661
// Create Date:    2016/12/12
// Design Name:    simulation
// Module Name:    CDBoost
// Project Name:   teamproject
// Target Devices: 
// Tool versions:   linux,DEV
//
//////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<iostream> 


#define Thread_Num 20

using namespace std;

pthread_mutex_t Device_mutex ;
 

struct PCB
{
    int tid;
    int priority;
    int waittime;
    int runtime;
    int arrivetime;
    int visited;
    int tempruntime;
    int cpu;
    
    
    public:
        int gettid()
        {
            return tid; 
        }
        
        int getcpu()
        {
            return cpu; 
        }
        
        int getwaittime()
        {
            return waittime;    
        }
        
        int getpriority()
        {
            return priority;
        }
        
        int getruntime()
        {
            return runtime;
        }
        
        int getarrivetime()
        {
            return arrivetime;
        }
        
        void setvisit(int a)
        {
            visited=a;
        }
        
        int getvisit()
        {
            return visited;
        }
        
        int gettempruntime()
        {
            return tempruntime;
        }
        
        void setwaittime(int n)
        {
            waittime = n;   
        }
        
        void settempruntime(int n)
        {
            tempruntime = tempruntime - n;
        }
}TCB[Thread_Num]; 


void t_init()
{
    int n;
    srand(time(NULL));
    for(n =0;n<Thread_Num;n++)
    {
    TCB[n].tid = n + 1;
    TCB[n].cpu=1+ + rand()%9;
    TCB[n].priority = 1 + rand()%9;
    TCB[n].runtime = 1 + rand()%9;
    TCB[n].arrivetime = 0;
    TCB[n].waittime = 0;
    TCB[n].visited =0;
    TCB[n].tempruntime = TCB[n].runtime;
    }
}


void *t_print(void *arg)
{
    int n = *(int *)arg;
    while(1)
    {
            pthread_mutex_lock(&Device_mutex);
            printf("Thread_%-2d: ",n);
            printf("tid:%-2d priority:%-2d runtime:%-2d cpu:%-2d \n",TCB[n-1].gettid(),TCB[n-1].priority,TCB[n-1].runtime,TCB[n-1].cpu);
            pthread_mutex_unlock(&Device_mutex);
            sleep(1);
        break;
    }

    pthread_exit(0);
}


void FCFS()
{
    cout<<"&&&&&&&&&&&&&&&&&&&&&&RR First come first service schedule function:"<<endl;
    int i,j;
    int start = 0;
    float waittime = 0;
    float avwait = 0;
    for(i=0;i<Thread_Num/2;i++)
    {
        for(j=0;j<Thread_Num;j++){
            if(TCB[j].getarrivetime()==i && TCB[j].getvisit()==0){
                printf("Thread: %-2d  Start: %-3d   Runtime: %-2d cpu: %-2d\n",TCB[j].gettid(),start,TCB[j].getruntime(),TCB[j].getcpu());
                waittime = waittime + (float)start;
                start = start + TCB[j].getruntime();
                TCB[j].setvisit(1);
            }
        }
    }
    avwait = waittime / (float)Thread_Num;
    printf("Total waitting time : %f\n",waittime);
    printf("Average waitting time : %f\n",avwait);
}


void SJF()
{
    for(int k=0 ;k<Thread_Num;k++)
    {
        TCB[k].setvisit(0);
    }
    cout<<"&&&&&&&&&&&&&&&&&&&&&&RR Shortest job first schedule function:"<<endl;
    int i,j;
    int start = 0;
    float waittime = 0;
    float avwait = 0;
    for(i=1;i<Thread_Num;i++)
    {
        for(j=0;j<Thread_Num;j++){
            if(TCB[j].getruntime()==i && TCB[j].getvisit()==0){
                printf("Thread: %-2d  Start: %-3d   Runtime: %-2d cpu: %-2d\n",TCB[j].gettid(),start,TCB[j].getruntime(),TCB[j].getcpu());
                waittime = waittime + (float)start;
                start = start + TCB[j].getruntime();
                TCB[j].setvisit(1);
            }
        }
    }
    avwait = waittime / (float)Thread_Num;
    printf("Total waitting time : %f\n",waittime);
    printf("Average waitting time : %f\n",avwait);
}


void RR(int r)
{
    cout<<"&&&&&&&&&&&&&&&&&&&&&& Round R schedule function:"<<endl;
    int start = 0;
    float waittime = 0;
    float avwait = 0;
    for(int i=0;i<Thread_Num;i++)
    {
        int totaltime = totaltime + TCB[i].getruntime();
        TCB[i].setvisit(0);
    }
    for(int j=0;j<20*Thread_Num;j=j+r)
    {
        int k = (j%(20*r))/r;
        if(TCB[k].gettempruntime() > 0){
            int tepruntime = r;
            if(TCB[k].gettempruntime()-r<=0){
                tepruntime = TCB[k].gettempruntime();
                TCB[k].setwaittime(start + tepruntime - TCB[k].getruntime());
                }
            printf("Thread: %-2d  Start: %-3d   Runtime: %-2d cpu: %-2d\n",TCB[k].gettid(),start,TCB[k].getruntime(),TCB[k].getcpu());
            start = start + tepruntime;
            TCB[k].settempruntime(r) ;
        }
    }
    for(int m=0;m<Thread_Num;m++)
    {
        waittime += TCB[m].getwaittime();
    }
    avwait = waittime / (float)Thread_Num;
    printf("Total waitting time : %f\n",waittime);
    printf("Average waitting time : %f\n",avwait);
}



void Priority()
{
    for(int k=0 ;k<Thread_Num;k++)
    {
        TCB[k].setvisit(0);
    }
    cout<<"&&&&&&&&&&&&&&&&&&&&&& Priority schedule function:"<<endl;
    int i,j;
    int start = 0;
    float waittime = 0;
    float avwait = 0;
    for(i=1;i<Thread_Num;i++)
    {
        for(j=0;j<Thread_Num;j++){
            if(TCB[j].getpriority()==i && TCB[j].getvisit()==0){
                printf("Thread: %-2d  Start: %-3d   Runtime: %-2d cpu: %-2d\n",TCB[j].gettid(),start,TCB[j].getruntime(),TCB[j].getcpu());
                waittime = waittime + (float)start;
                start = start + TCB[j].getruntime();
                TCB[j].setvisit(1);
            }
        }
    }
    avwait = waittime / (float)Thread_Num;
    printf("Total waitting time : %f\n",waittime);
    printf("Average waitting time : %f\n",avwait);
}




void *Children(void*)
{
    int ret[Thread_Num];
    t_init();
        pthread_t tid[Thread_Num];
    pthread_mutex_init(&Device_mutex,NULL);
        int i,j;

        for(i=0;i<Thread_Num;i++)
        {
        int k =i+1;

            ret[i] = pthread_create(&tid[i],NULL,&t_print, &k);
        if(ret[i] == 0) {
            sleep(1);
        }
            else{ 
            printf("error:Thread_%-2d failed!\n",i+1);

        }   
        }
        for(j=0;j<Thread_Num;j++)
            pthread_join (tid[i], NULL);
    pthread_mutex_destroy(&Device_mutex);
    pthread_exit(0);
}


int main()
{
    int ret1;
    pthread_t tid1;
    ret1 = pthread_create(&tid1,NULL,&Children,NULL);
    if(ret1 == 0) 
    {
        printf("Main Thread done!\n");
        sleep(20);
    }
        else{ 
        printf("Main thread failed!\n");

    }   
 
    FCFS();
    SJF();
    cout<<"Please enter RR time:\n";//Request RR time
    int rr;
    scanf("%d",&rr);
    RR(rr);
    Priority();
        return 0;
}
