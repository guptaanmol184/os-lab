#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

sem_t mutex1;
sem_t mutex2;
pthread_t t1,t2;

void *worker1(void* params);
void *worker2(void* params);

void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("received SIGINT\n");
        pthread_kill(t1, SIGTERM);
        sem_post(&mutex1);
        sem_post(&mutex2);
    }
}

int main()
{
    // register signal with the kernel
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    sem_init(&mutex1, 0, 1);
    sem_init(&mutex2, 0, 1);

    pthread_attr_t attr;
    pthread_attr_init(&attr);


    pthread_create(&t1, &attr, &worker1, NULL);
    pthread_create(&t2, &attr, &worker2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

void *worker1(void* params)
{
    printf("t1 running\n");

    fflush(stdout);
    sem_wait(&mutex1);
    printf("t1 acquired mut 1\n");
    sleep(2);
    fflush(stdout);
    printf("t1 will try for mut 2\n");
    fflush(stdout);
    sem_wait(&mutex2);
    sem_post(&mutex1);
    sem_post(&mutex2);

    pthread_exit(0);
}

void *worker2(void* params)
{
    printf("t2 running\n");
    fflush(stdout);

    sem_wait(&mutex2);
    printf("t2 acquired mut 2\n");
    fflush(stdout);
    printf("t2 will try for mut 1\n");
    fflush(stdout);
    sem_wait(&mutex1);
    sem_post(&mutex1);
    sem_post(&mutex2);

    printf("t2 zinda hai");
    fflush(stdout);

    pthread_exit(0);
}
