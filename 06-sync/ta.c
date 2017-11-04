#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int n = 4;   // total number of chairs = TA chair + waiting chairs
int student_count = 0; // total number of students occupying chairs

sem_t mutex;
sem_t student;
sem_t ta;
sem_t student_done;
sem_t ta_done;

void* student_runner(void * params);
void* ta_runner(void * params);

void solve_doubt(int id);
void student_talk(int id);
void ta_talk();
void program(int id);

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&student, 0, 0);
    sem_init(&ta, 0, 0);
    sem_init(&student_done, 0, 0);
    sem_init(&ta_done, 0, 0);

    int student_count=0;
    printf("Enter total number of students for simulation: ");
    scanf("%d", &student_count);

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    //ta thread
    pthread_t ta_thread;
    pthread_create(&ta_thread, &attr, &ta_runner, NULL);

    //student threads
    pthread_t student_thread[student_count];
    for( int i=0; i<student_count; i++)
        pthread_create(&student_thread[i], &attr, &student_runner, (void*)i);

    //join students
    for( int i=0; i<student_count; i++)
        pthread_join(student_thread[i], NULL);

    //join ta
    pthread_join(ta_thread, NULL);

    return 0;
}

void* student_runner(void * params) {
    void* return_nothing = NULL;
    int id = (int) params;
a:
    program(id);

    sem_wait(&mutex);
    if(student_count == n) {
        printf("Seats are full ;(\n");
        sem_post(&mutex);
        goto a;
    }
    else {
        ++student_count;
        printf("Student %d occupied a chair. Chair occupied count = %d.\n", id, student_count);
    }
    sem_post(&mutex);

    sem_post(&student);
    sem_wait(&ta);

    solve_doubt(id);

    sem_post(&student_done);
    sem_wait(&ta_done);

    sem_wait(&mutex);
    --student_count;
    printf("Student %d got his doubt cleared. Chair occupied count = %d.\n", id, student_count);
    sem_post(&mutex);

    pthread_exit(0);
    return return_nothing;
}

void solve_doubt(int id) {
    printf("TA is solving doubt of student %d.\n", id);
    int r = (rand() % 5)+1;
    sleep(r);  // solve doubt
    printf("TA solved doubt of student %d in %d seconds.\n", id, r);
}

void* ta_runner(void * params) {

    while(1) {
        sem_wait(&student);
        sem_post(&ta);

        //ta_talk();

        sem_post(&ta_done);
        sem_wait(&student_done);
        printf("Ta is sleeping.\n");
    }
}

void student_talk(int id) {
    int r = (rand() % 7)+1;
    for(int i=0; i<r; i++)
        printf("S%d: Hello\n", id);
    sleep(r);
}

void ta_talk() {
    int r = (rand() % 4)+1;
    for(int i=0; i<r; i++)
        printf("Vaani: DIE!\n");
    sleep(r);
}

void program(int id) {
    printf("S%d is programming.\n", id);
    int r = (rand() % 5)+1;
    sleep(r);
}

