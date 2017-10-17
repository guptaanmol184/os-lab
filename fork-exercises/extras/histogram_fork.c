#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <color.h>

#define STRLEN 2048

int main()
{
    pid_t pid;
    char input_str[STRLEN];
    int lower_char[26] = {0};
    int upper_char[26] = {0};
    memset(lower_char, 0, 27*sizeof(int));
    memset(upper_char, 0, 27*sizeof(int));

    printf("Enter a string: ");
    fgets(input_str, STRLEN, stdin);

    pid = vfork();
    switch(pid)
    {
        case -1: // error
        {
            printf("Fork error.\n");
            exit(EXIT_FAILURE);
        }

        case 0: // child
        {
            // generate the character count
            for(int i=0; i<STRLEN && input_str[i]!='\0'; i++)
            {
                if(input_str[i]>='a' && input_str[i]<='z')
                    ++lower_char[(int)input_str[i] - 97];
                if(input_str[i]>='A' && input_str[i]<='Z')
                    ++upper_char[(int)input_str[i] - 65];
            }
            break;
        }
        default: // parent
        {
            wait(NULL);
            // print count
            // lower chars
            for( int i=0; i<26; i++)
            {
                if(lower_char[i]!= 0)
                {
                    printf(ANSI_COLOR_CYAN "%c: ", 97+i);
                    for(int j=0; j<lower_char[i]; j++)
                        printf(ANSI_COLOR_GREEN "#" ANSI_COLOR_RESET);
                    printf("\n");
                }
            }
            // uppper chars
            for( int i=0; i<26; i++)
            {
                if(upper_char[i]!= 0)
                {
                    printf(ANSI_COLOR_CYAN "%c: ", 65+i);
                    for(int j=0; j<upper_char[i]; j++)
                        printf(ANSI_COLOR_GREEN "#" ANSI_COLOR_RESET);
                    printf("\n");
                }
            }
        }
    }

    exit(EXIT_SUCCESS);
}
