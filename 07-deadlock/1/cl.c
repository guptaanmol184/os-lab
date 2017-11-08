#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define s1 "/mutex1"
#define s2 "/mutex2"

int main(void)
{
    printf("clearing all semaphores...\n");
    sem_unlink(s1);
    sem_unlink(s2);
    printf("cleared.\n");
}
