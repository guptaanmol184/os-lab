#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <string.h>

int main()
{
    int shm_fd;
    char *start="ds";
    char isPalin[20];
    char *ptr;
    int SIZE=1024;
    
    shm_fd = shm_open(start,O_RDWR,0666);

    ptr = mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);

    strcpy(isPalin,ptr);
    printf("%s", isPalin);
    int len=strlen(ptr);
    int i=0;
    while(i < len/2)
    {
        isPalin[i]=isPalin[len-i-1];
        i++;
    }

    if(strcmp(isPalin,ptr)==0)
        printf("IS PALINDROME!!\n");
    else
        printf("NO!!!\n");


    shm_unlink(start);    

}