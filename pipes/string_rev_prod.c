#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
    int shm_fd;
    char *start="ds";
    char *palin = "malayalam";
    void *ptr;
    int SIZE=1024;
    
    shm_fd = shm_open(start, O_CREAT|O_RDWR,0666);

    ftruncate(shm_fd,SIZE);

    ptr = mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return -1;
    }

    sprintf((char*)ptr,"%s",palin);
    ptr+=strlen(palin);

}