#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid;
    
    pid = fork();
    int to_number=100;

    if (pid < 0)
    {
        printf("Fork failed...");
    }
    else if (pid == 0)
    {
        // Child block
        // Odd series
        for( int i=1; i<to_number; i+=2)
        {
            printf("\tC:%d\n", i);
        }
    }
    else
    {
        wait(NULL);
        // Parent block
        // Even series
        for( int i=0; i<to_number; i+=2)
        {
            printf("p:%d\n", i);
        }
    }
}
