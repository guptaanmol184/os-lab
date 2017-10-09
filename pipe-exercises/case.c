#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define BS 20
#define RE 0
#define WE 1

int main()
{
    char wm[BS] = "Hello pipe";
    char rm[BS] = "";
    int fd[2];
    pid_t pid;

    if(pipe(fd) == -1)
        perror("pipe: ");

    pid = fork();

    if(pid<0)
        perror("fork: ");

    if(pid>0)
    {
        close(fd[RE]);
        write(fd[WE], wm, strlen(wm)+1);
        close(fd[WE]);
    }
    if(pid == 0)
    {
        close(fd[WE]);
        read(fd[RE], rm, BS);
        close(fd[RE]);

        printf("Recieved message from parent: %s\n", rm);
    }

}
