/*
qiyang xia
100837661
assignment1 partC.c 
*/ 

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<semaphore.h>
#include<time.h>

#define seats 3//three seats in waiting room
#define MAX 8//custom max number can be define whatever

int waitingN=0;
sem_t sem_cust;
sem_t sem_barber;
sem_t sem_mutex;

void *barber(void)
{
  int i=0;
  while(1)
  {
    sem_wait(&sem_cust);
    sem_wait(&sem_mutex);
    waitingN--;
    sem_post(&sem_mutex);
    
    i++;
    printf("customer #%d is doing hair cut.\n",i);
    sleep((rand()%20)/5);
    sem_post(&sem_barber);
    printf("customer #%d is done.\n",i);
   }
}

void *custom(void *p)
{
  int N;
  int cust_count;
  sem_wait(&sem_mutex);
  cust_count=*(int*)p;
  printf("customer #%d come in.\n",cust_count);
  if(waitingN<seats)
  {
    sem_getvalue(&sem_barber,&N);
    waitingN++;
    if(waitingN==1&&N==1)
    {
    printf("barber is sleeping,need wake up hime.\n");
    printf("barber wake up.\n");
    }
    else if(waitingN>0)
    {
      if(waitingN==1)
         printf("only one customer in waiting room.\n");
      else
         printf("have %d customers in waiting room.\n",waitingN);
    }
    sem_post(&sem_mutex);
    sem_post(&sem_cust);
            sem_wait(&sem_barber);
    sleep(3);
  }
  else
  {
    printf("there is no seats any more, customer leave.\n");
    sem_post(&sem_mutex);
    
    pthread_exit(0);
  }
  pthread_exit(0);
}

int main(void)
{
  int k;
  pthread_t Pbarber;
  pthread_t Pcust[8];

  sem_init(&sem_cust,0,0);
  sem_init(&sem_barber,0,1);
  sem_init(&sem_mutex,0,1);

  if(pthread_create(&Pbarber,NULL,(void*)barber,NULL)!=0)
  {
     return 0;
  }
  printf("barber open the shop.\n");
  printf("shop have no customer now, barber go to sleep.\n");
  for(k=1;k<=MAX;k++)
  {
     if(pthread_create(&(Pcust[k-1]),NULL,(void*)custom,&k)!=0)
     {
       exit(0);
     }
     srand(time(0));
     sleep(rand()%3+1);
   }
   for(k=0;k<MAX;k++)
     pthread_join(Pcust[k],NULL);

     sleep(3);

     return 0;
}
    


