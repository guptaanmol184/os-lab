#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *stream_read = popen("ls","r");
    FILE *stream_write = popen("less","w");

    char buffer[1024];
    while(!feof(stream_read) && !ferror(stream_read) && (fgets(buffer, sizeof(buffer), stream_read)!=NULL))
    {
        fputs(buffer, stream_write);
    }

    pclose(stream_read);
    return pclose(stream_write);    
}