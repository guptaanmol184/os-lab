#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define RD_END 0
#define WR_END 1 

int main(void)
{
        int     fd[2];
        char    readbuffer[1024];
        memset(readbuffer,0,1024);
        pid_t childpid; 

        pipe(fd);

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid == 0)
        {
                close(fd[RD_END]);
                dup2(fd[WR_END],1);          
                system("ls");
                //execlp("ls","ls",NULL);
                //      write(fd[1], string, (strlen(string)+1));
                exit(0);
        }
 
        else
        {                  
                wait(NULL);
                /*char c;*/
                int i=0,j=0;
                close(fd[WR_END]);                 
                while((j=read(fd[RD_END], readbuffer, sizeof(readbuffer))) > 0)
                {       
                        i=0;
                        while(i<j)
                        {
                                readbuffer[i]=toupper(readbuffer[i]);
                                if(readbuffer[i]=='\n')
                                    readbuffer[i]=' ';
                                ++i;
                        }                                
                }
                
           /*     while(c=toupper(readbuffer[i]) != '\n')
                        readbuffer[i]=c;*/

                printf("Received string: %s ", readbuffer);
                close(fd[RD_END]);      
        }

        return(0);
}
