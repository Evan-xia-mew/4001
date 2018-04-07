/*
qiyang xia
100837661
assignment1 partA.c 
*/ 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>

#define seats 3

pthread_t bar;
pthread_t cust;
pthread_t check;
pthread_mutex_t lock;
pthread_cond_t wakeup;
pthread_cond_t gener;
pthread_cond_t cuting;
pthread_cond_t done;
    
int current=0;

void *barber()
{
    while(1){
    	pthread_mutex_trylock(&lock);
		if(current==0){
			printf("there is no customer now, barber go to sleep.\n");
			fflush(stdout);
			pthread_cond_wait(&wakeup, &lock);
		}else{
			printf("barber begins cutting hair.\n");
			fflush(stdout);
            sleep((rand()%20)/5);
            current--;
      printf("hair cut complete,customer who completed leave.\n");
			fflush(stdout);
			pthread_cond_signal(&cuting);
			pthread_cond_wait(&wakeup, &lock);		
		}
		pthread_mutex_unlock(&lock);
	}
	  pthread_exit(0);
}


	
void *checkQueue()
{
	
while (1) {
		int temp = 0;
		int customerM = rand() % 3;
		pthread_mutex_trylock(&lock);
		pthread_cond_wait(&gener, &lock);
		temp=(customerM+current)-3;
		current += customerM;
	    printf("%d customer go to waiting room.there are %d Customer in shop.\n",customerM,current);
        fflush(stdout);
        printf("customer checking seats.\n");
        fflush(stdout);
        sleep(1);
		if(current >seats){
			current-=temp;
			printf("there is no seats any more, extra %d customer leave.\n",temp);
			fflush(stdout);
			sleep(3);
		}		
		if(current != 0){
			pthread_cond_signal(&wakeup);
			pthread_cond_wait(&cuting, &lock);		
		}
		pthread_cond_signal(&done);	
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

void *customer()
{
  while(1)
  {
    pthread_mutex_trylock(&lock);  
    sleep(3);
    pthread_cond_signal(&gener);
	pthread_cond_wait(&done, &lock);
	pthread_mutex_unlock(&lock);
  }
  pthread_exit(0);
}

int main()
{
    printf("barber open the shop.\n");
    
    pthread_mutex_t  lock=PTHREAD_MUTEX_INITIALIZER;  

    pthread_create(&bar, NULL, &barber, NULL);
    pthread_create(&cust, NULL, &customer, NULL);
    pthread_create(&check, NULL, &checkQueue, NULL);
    
    pthread_join(bar, NULL);
    pthread_join(check, NULL);
    pthread_join(cust, NULL);
    
    pthread_mutex_destroy(&lock);

    return 0;
}
