#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    pid_t pid = fork();

    if(pid==0)
    {
        sleep(2);
        printf("orphan child\n");
    }
    else
    {
        printf("dying parent");
    }
}

