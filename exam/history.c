#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define CMD_SIZE 100
#define TOTAL_OPTS 20


// parse the input line
void  parse_line(char *line, char* argv[TOTAL_OPTS])
{
    int i=0;

    while(*line != '\0')
    {
        // if it is a white space char
        // replace with null
        while(*line == '\n' || *line == '\t' || *line == ' ')
        {
            *line='\0';
            line++;
        }

        // if it is a non white space char
        // store the address of first non white space char in argv
        argv[i] = line;
        i++;
        line++;

        // continue till we encounter white space
        while(*line !='\n' && *line !='\t' && *line != ' ' && *line != '\0')
           line++;
    }

    argv[i]='\0';


    // print argv
    //for(int i=0; argv[i] != '\0'; i++)
    //    printf("%s\n", argv[i]);
}

//implement execute
int execute(char* argv[TOTAL_OPTS])
{
    // fork and execute the command
    pid_t pid;
    int rv = 1;
    switch(pid = fork())
    {
        case -1:
            // fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            // child - execute command
            return(execvp(argv[0], argv));
        default:
            // parent
            wait(&rv);
            return(WEXITSTATUS(rv));
    }
}

// implement history
void history(char* saveline, char* argv[TOTAL_OPTS])
{
    if(strcmp(argv[0], "history") == 0)
    {
        FILE *fp = fopen("history.txt", "r");
        int line_no = 1;
        char line[CMD_SIZE];

        while(fgets(line, CMD_SIZE, fp) != NULL)
        {
            printf("%4d %s", line_no, line);
            line_no++;
        }
        fclose(fp);
    }
    else if(argv[0][0] == '!')
    {
        if(argv[0][1] == '!')
        {
            FILE *fp = fopen("history.txt", "r");
            char line[CMD_SIZE];
            char cmd[CMD_SIZE];
            char* cmd_argv[TOTAL_OPTS];

            while(fgets(line, CMD_SIZE, fp) != NULL)
            {
                // save command
                strcpy(cmd, line);
            }
            fclose(fp);
            cmd[strlen(cmd)-1]='\0'; // remove trailing '\n'
            printf(" %s\n", cmd);
            // parse cmd and execute
            parse_line(cmd, cmd_argv);
            execute(cmd_argv);
        }
        else
        {
            // count total lines
            FILE *fp = fopen("history.txt", "r");
            int line_no = 1;
            char line[CMD_SIZE];

            while(fgets(line, CMD_SIZE, fp) != NULL)
                line_no++;
            fclose(fp);

            
            // print last x lines depending on !x
            // get x
            char cnum[10];
            strcpy(cnum, argv[0]+1);

            int num;
            num = atoi(cnum);
            //printf("%d\n", num);

            // check lines
            if(num >= line_no)
                printf("Asked for too many lines!!\n");
            
            // print required lines
            {
                int diff = line_no-num;
                // print lines
                FILE *fp = fopen("history.txt", "r");
                int line_no = 1;
                char line[CMD_SIZE];

                while(fgets(line, CMD_SIZE, fp) != NULL)
                {
                    if(line_no>=diff)
                        printf("%s", line);
                    line_no++;
                }
                fclose(fp);
            }
            


        }
    }
    else
    {
        int rv = execute(argv);

        // write to file on success
        if(rv == 0)
        {
            FILE *fp;
            fp = fopen("history.txt", "a");
            fprintf(fp, "%s\n", saveline);
            fclose(fp);
        }
        
        printf("%d ",rv);
    }

}

int main()
{
    char* line;
    char* saveline;
    char* argv[TOTAL_OPTS];
    line = calloc(1, CMD_SIZE);
    saveline = calloc(1, CMD_SIZE);

    while(1)
    {
        // print prompt
        printf("$> ");
        line = fgets( line, CMD_SIZE, stdin);
        line[strlen(line)-1]='\0'; // remove trailing '\n'
        strcpy(saveline, line);

        // check line == exit
        if(strcmp(line, "exit") == 0)
            exit(EXIT_SUCCESS);

        parse_line(line, argv);

        // print argv
        //for(int i=0; argv[i] != '\0'; i++)
        //    printf("%s\n", argv[i]);

        // call history
        history(saveline, argv);
    }
}
