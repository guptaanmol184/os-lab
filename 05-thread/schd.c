#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* runner( void* params);
int main()
{
    int choice;
    printf("Select a sched policy: 1) RR 2) FIFO 3)OTHER: ");
    scanf("%d", &choice);

    pthread_t tid[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // set schedule policy
    switch(choice)
    {
        case 1:
            printf("Set schedule policy RR\n");
            pthread_attr_setschedpolicy(&attr, SCHED_RR);
            break;
        case 2:
            printf("Set schedule policy FIFO\n");
            pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
            break;
        case 3:
            printf("Set schedule policy OTHER\n");
            pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
            break;
    }

    pthread_create(&tid[0], &attr, &runner, "./inv");
    pthread_create(&tid[1], &attr, &runner, "./mul");
    pthread_create(&tid[2], &attr, &runner, "./qs");

    // join
    for( int i=0; i<3; i++)
        pthread_join(tid[i], NULL);

    return 0;
}

void* runner (void* params)
{
    char* exe = (char*) params;
    system(exe);
    pthread_exit(0);
}
