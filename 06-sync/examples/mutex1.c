#define BUFFER_SIZE 5
/*main.c*/
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
//#include "buffer.h"
typedef int buffer_item;
#define RAND_DIVISOR 100000000
#define TRUE 1
/*mutex lock*/
pthread_mutex_t mutex;
/*the semaphores*/
sem_t full,empty;
/*the buffer*/
buffer_item buffer[BUFFER_SIZE];
/*buffer counter*/
int counter;
pthread_t tid;//thread id
pthread_attr_t attr;//set of thread attributes
void *prodcer(void *param);/*producer thread*/
void *consumer(void *param);/*consumer thread*/
void initializeData()
{
/*create the mutex lock*/
pthread_mutex_init(&mutex,NULL);

/*create the full semaphore and initialize to 0*/
 sem_init(&full,0,0);
 /*create the empty semaphore and initialize to BUFFER_SIZE*/
 sem_init(&empty,0,BUFFER_SIZE);
 /*get the default attribute*/
 pthread_attr_init(&attr);
 /*init buffer*/
 counter=0;
 }
 
 /*producer thread*/
 
 void *producer(void *param)
 {
 buffer_item item;
 
 while(TRUE)
 {
 /*sleep for a random period of time*/
 int rNum=rand()/RAND_DIVISOR;
 sleep(rNum);
/*generate  a random number*/
item =rand();
/*acquire the empty lock*/

/*acquire the mutex lock*/
pthread_mutex_lock(&mutex);
sem_wait(&empty);
if(insert_item(item))
{
fprintf(stderr,"producer report error condition\n");

}
else
{
printf("producer produced %d\n",item);
}
sem_post(&full);
/*release the mutex lock*/
pthread_mutex_unlock(&mutex);
/*signal full*/

}
}
/*consumer thread*/
void *consumer(void *param)
{
buffer_item item;
while(TRUE)
{
/*sleep for a random period of time*/
int rNum =rand()/RAND_DIVISOR;
sleep(rNum);
/*acquire the full lock*/

/*acquire the mutex lock*/
pthread_mutex_lock(&mutex);
sem_wait(&full);
if(remove_item(&item))
{
fprintf(stderr,"consumer report error condition\n");
}
else
{
printf("consumer consumed %d\n",item);
}
sem_post(&empty);

/*release the mutex lock*/
pthread_mutex_unlock(&mutex);
/*signal empty*/
}
}
/*add an item to the buffer*/
int insert_item(buffer_item item)
{
/*when the buffer is not full add the item and increment the counter*/
if(counter<BUFFER_SIZE)
{
buffer[counter]=item;
counter++;
return 0;
}





else
{
/*error the bufer is full*/
return -1;
}
}/*remove an item from the buffer*/
int remove_item(buffer_item *item)
{
/*when the buffer is not empty remove the item and decrement the counter*/
if(counter>0)
{
*item=buffer[(counter-1)];
counter--;
return 0;
}
else
  {
  /*error buffer empty*/
      return -1;
}
}
int main(int argc,char *argv[])
{
/*loop counter*/
int i;
/*verify the correct no of arguments were passed in */
if(argc!=4)
{
  fprintf(stderr,"USAGE:./main.out <INT> <INT> <INT>\n");
}
int mainSleepTime=atoi(argv[1]);/*time in seconds for main to sleep*/
int numProd=atoi(argv[2]);/*number of producer threads*/
int numCons=atoi(argv[3]);/*number of consumer threads*/
/*initialize the app*/
initializeData();
/*create the producer threads*/
for(i=0;i<numProd;i++)
{
/*create the thread*/
pthread_create(&tid,&attr,producer,NULL);
}
/*create the consumer threads*/
for(i=0;i<numCons;i++)
{
/*create the thread*/
pthread_create(&tid,&attr,consumer,NULL);
}
/*sleep for the specified amount of time in milliseconds*/
sleep(mainSleepTime);
/*exit program*/
printf("exit the program\n");
exit(0);
}
