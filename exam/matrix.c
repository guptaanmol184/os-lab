#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAT_SIZE 4
#define RE 0
#define WE 1

void print_mat(int mat[MAT_SIZE][MAT_SIZE])
{
    printf("Input matrix:\n");
    
    for( int i=0; i < MAT_SIZE; i++)
    {
        printf(" ");
        for(int j=0; j<MAT_SIZE; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

int check_square(int mat[MAT_SIZE][MAT_SIZE])
{
    int row_sums[MAT_SIZE];
    int col_sums[MAT_SIZE];
    int diag_sums[2] = {0,0};

    // print row and col and diag sums
    for(int i=0; i<MAT_SIZE; i++)
    {
        col_sums[i]=0;
        row_sums[i]=0;
        for(int j=0; j<MAT_SIZE; j++)
        {
            row_sums[i]+=mat[i][j];
            col_sums[i]+=mat[j][i];

            if(i == j)
                diag_sums[0]+=mat[i][j];
            if(i+j == MAT_SIZE-1)
                diag_sums[1]+=mat[i][j];
            
        }
        //printf("%d:%d ", row_sums[i], col_sums[i]);
    }
    //printf("\n");
    //printf("%d:%d", diag_sums[0], diag_sums[1]);

    // check equivalence
    int com_val = row_sums[0];

    for(int i=0; i<MAT_SIZE; i++)
        if(row_sums[i] != com_val)
            return 0;

    for(int i=0; i<MAT_SIZE; i++)
        if(col_sums[i] != com_val)
            return 0;

    for(int i=0; i<2; i++)
        if(diag_sums[i] != com_val)
            return 0;

    return 1;
}

int main()
{
    pid_t pid;
    int fd[2];
    int mat[MAT_SIZE][MAT_SIZE];
    int result;

    pipe(fd);

    switch(pid = fork())
    {
        case -1:
            perror("fork: ");
            exit(EXIT_FAILURE);
        case 0:
            // child code
            close(fd[RE]); // we are not reading
            printf("Enter a %dx%d square matrix\n", MAT_SIZE, MAT_SIZE);
            for( int i=0; i < MAT_SIZE; i++)
            {
                printf("Row %d\n", i);
                for(int j=0; j<MAT_SIZE; j++)
                {
                    printf(" Value %d: ", j);
                    scanf("%d", &mat[i][j]);
                }
            }
            print_mat(mat);

            write(fd[WE], (void*)mat, sizeof(mat));  //write matrix to pipe
            close(fd[WE]); // close the write end
            break;
        default:
            // parent code

            wait(NULL); // wait for child to finish reading input
            close(fd[WE]);  // read from pipe
            read(fd[RE], (void*)mat, sizeof(mat));
            //print_mat(mat);

            if(check_square(mat))
                printf("Input matrix is a magic square\n");
            else
                printf("Input matrix is not a magic square\n");

            exit(EXIT_SUCCESS);
    }


    
}
