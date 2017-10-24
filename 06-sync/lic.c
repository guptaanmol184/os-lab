/*
 * License simulator using semaphores
 */

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define LICENSE_COUNT 10
sem_t license_admin;
sem_t mutex;

void* use_license(void * params);

int main() {
    sem_init(&license_admin, 0, LICENSE_COUNT);
    sem_init(&mutex, 0, 1);

    int no_of_students;
    printf("Enter the number of students using the license: ");
    scanf("%d", &no_of_students);

    pthread_t tid[no_of_students];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for( int i=0; i<no_of_students; i++) {
        pthread_create(&tid[i], &attr, &use_license, NULL);
    }

    for( int i=0; i<no_of_students; i++) {
        pthread_join(tid[i], NULL);
    }

}

void* use_license(void * params) {
    int lcount;

    // lock control of one thread to read "license_admin" semaphore value
    sem_wait(&mutex);
    sem_wait(&license_admin);
    sem_getvalue(&license_admin, &lcount);
    printf("License acquired. Remaining license: %d.\n",  lcount);
    sem_post(&mutex);
    // unlock control

    // use license for a random amount of time
    int r = rand() % 5;
    sleep(r);

    // return license
    sem_post(&license_admin);

    printf("License released.\n");

    pthread_exit(0);
}
