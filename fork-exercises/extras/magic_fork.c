#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define SIZE 3 // works only when n is odd
int magicSquare[SIZE][SIZE]; // store magic square

void generateSquare(int n);
int verify_magic(int mat[SIZE][SIZE]);
int check_square(int mat[SIZE][SIZE]);

int main()
{
    pid_t pid;
    pid = fork();
    switch(pid)
    {
        case -1: // error
            printf("Fork failed");
            break;
        case 0: // child
	            generateSquare (SIZE);
                break;
        default: // parent
            {
                wait(NULL); // wait for child to generate a matrix
                if(check_square(magicSquare))
                    printf("Generated matrix is a magic square.\n");
                else
                    printf("Generated matrix is NOT a magic square.\n");
            }
    }
    return 0;
}


// A function to generate odd sized magic squares
void generateSquare(int n)
{
	// set all slots as 0
	memset(magicSquare, 0, sizeof(magicSquare));

	// Initialize position for 1
	int i = n/2;
	int j = n-1;

	// One by one put all values in magic square
	for (int num=1; num <= n*n; )
	{
		if (i==-1 && j==n) //3rd condition
		{
			j = n-2;
			i = 0;
		}
		else
		{
			//1st condition helper if next number 
			// goes to out of square's right side
			if (j == n)
				j = 0;
			//1st condition helper if next number 
			// is goes to out of square's upper side
			if (i < 0)
				i=n-1;
		}
		if (magicSquare[i][j]) //2nd condition
		{
			j -= 2;
			i++;
			continue;
		}
		else
			magicSquare[i][j] = num++; //set number

		j++; i--; //1st condition
	}

	// Print magic square
	/*printf("The Magic Square for n=%d:\nSum of each row or column %d:\n\n",*/
			/*n, n*(n*n+1)/2);*/
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			printf("%3d ", magicSquare[i][j]);
		printf("\n");
	}
}

int check_square(int mat[SIZE][SIZE])
{
    int row_sums[SIZE];
    int col_sums[SIZE];
    int diag_sums[2] = {0,0};

    // print row and col and diag sums
    for(int i=0; i<SIZE; i++)
    {
        col_sums[i]=0;
        row_sums[i]=0;
        for(int j=0; j<SIZE; j++)
        {
            row_sums[i]+=mat[i][j];
            col_sums[i]+=mat[j][i];

            if(i == j)
                diag_sums[0]+=mat[i][j];
            if(i+j == SIZE-1)
                diag_sums[1]+=mat[i][j];
            
        }
        //printf("%d:%d ", row_sums[i], col_sums[i]);
    }
    //printf("\n");
    //printf("%d:%d", diag_sums[0], diag_sums[1]);

    // check equivalence
    int com_val = row_sums[0];

    for(int i=0; i<SIZE; i++)
        if(row_sums[i] != com_val)
            return 0;

    for(int i=0; i<SIZE; i++)
        if(col_sums[i] != com_val)
            return 0;

    for(int i=0; i<2; i++)
        if(diag_sums[i] != com_val)
            return 0;

    return 1;
}
