# include <stdio.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>

#define MAX_LENGTH 1024

void  parse(char *line, char **argv)
{

      // change the trailing newline to '\0'
      // fgets fucks this up
      /*printf("strlen: %d, line0: %c", strlen(line), line[0]);*/
      line[strlen(line) - 1] = '\0';

     while (*line != '\0')  /* if not the end of line ....... */ 
     {       
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */

          while (*line != '\0' && *line != ' ' &&  *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void  execute(char **argv)
{

     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0)
     {
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0)
     {
          if (execvp(*argv, argv) < 0)
          {
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else
     {
          wait(&status);
          printf("%d ", status); 
     }
}

char* history(char* line)
{
    // case 1 - print history file contents
    // print history file with numbers
    if( strncmp(line, "history", 7) == 0)
    {
        FILE *fp;
        char hist_cmd[MAX_LENGTH];
        int line_ctr=1;

        fp = fopen("ash_history.txt", "r");

        if(fp == NULL)
            printf("History file does not exist");


        while(fgets(hist_cmd, MAX_LENGTH, fp) != NULL)
        {
            printf("%4d %s", line_ctr, hist_cmd);
            line_ctr++;
        }


        fclose(fp);

    }

    // case 2 - execute one of previous commands
    if( line[0] == '!')
    {
        // execute last executed command
        if( line[1] == '!')
        {
            FILE *fp;
            char hist_cmd[MAX_LENGTH];
            char save_cmd[MAX_LENGTH];
            int line_ctr=1;

            fp = fopen("ash_history.txt", "r");

            if(fp == NULL)
                printf("History file does not exist");


            while(fgets(hist_cmd, MAX_LENGTH, fp) != NULL)
            {
                strcpy(save_cmd, hist_cmd);
                line_ctr++;
            }

            printf("%s", save_cmd);

            // execute save_cmd
            line = save_cmd;

            fclose(fp);
        }

        // execute command specified by the number
        else
        {
            char ascii_line_number[6];
            int int_line_number;

            for(int i=1; line[i] != ' ' && line[i] != '\t' && line[i] != '\n'; i++)
                ascii_line_number[i-1] = line[i];


            // convert ascii to int
            int_line_number = atoi(ascii_line_number);

            // get that line
            FILE *fp;
            char hist_cmd[MAX_LENGTH];
            int line_ctr=1;

            fp = fopen("ash_history.txt", "r");

            if(fp == NULL)
                printf("History file does not exist");

            // find that line
            while(fgets(hist_cmd, MAX_LENGTH, fp) != NULL && line_ctr!= int_line_number)
                line_ctr++;

            // print hist_cmd
            if(line_ctr == int_line_number)
                printf("%s", hist_cmd);
            else
                printf("line number out of range");

            // execute hist_cmd command
            line = hist_cmd;
        }

    }


    // case 3 - not a history function
    // Add to history file
    // if string is not empty
    else
    {
        
        //save only if line is not empty
        if( line[0] != ' ' && line[0] != '\t' && line[0] != '\n' && (strncmp(line, "history", 7) != 0))
        {
            FILE *fp;
            fp = fopen("ash_history.txt", "a");
            fputs(line, fp);
            fclose(fp);

            // execute
        }
    }

    return line;

}
void  main(void)
{
     char  *line;       /* the input line                 */
     line = (char* )malloc(sizeof(char) * MAX_LENGTH);
     char  *argv[64];              /* the command line argument      */

     while (1)
     {                             /* repeat until done ....         */
          printf("ash>> ");           /*   display a prompt             */

          fgets(line, MAX_LENGTH, stdin);              /*   read in the command line     */

          line = history(line);

          /*if( strncmp(line, "history", 7) == 0)*/
              /*continue;*/

          parse(line, argv);       /*   parse the line               */

          /*printf("%s", line);*/
          if (strcmp(argv[0], "exit") == 0)  /* is it an "exit"?     */
               exit(0);            /*   exit if it is                */
          execute(argv);          // [> otherwise, execute the command <]
     }
}
