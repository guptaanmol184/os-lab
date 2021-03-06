#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5 // no of philosopher
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+1)%N
#define RIGHT (ph_num+4)%N

sem_t mutex; // overall sync
sem_t S[N]; // spoon array. 0 means ublocked and 1 means blocked.


void *philospher(void *num);
void take_fork(int);
void put_fork(int);

int main()
{
    int i;
    pthread_t thread_id[N];
    sem_init(&mutex,0,1);
    for(i=0;i<N;i++)
        sem_init(&S[i],0,0);

    for(i=0;i<N;i++)
    {
        pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]);
        printf("Philosopher %d is thinking\n",i+1);
    }
    
    for(i=0;i<N;i++)
        pthread_join(thread_id[i],NULL);
}

void *philospher(void *num)
{
    while(1)
    {
        int *i = num;
        //sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}
   
void take_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = HUNGRY;
    printf("Philosopher %d is Hungry\n",ph_num+1);
    test(ph_num);
    sem_post(&mutex);
    sleep(3);
    sem_wait(&S[ph_num]);
}

void put_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n",ph_num+1,LEFT+1,ph_num+1);
    printf("Philosopher %d is thinking\n",ph_num+1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}
