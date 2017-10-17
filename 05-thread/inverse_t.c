#include<stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 4

struct mat
{
  int matrix[N][N];
  float inv[N][N];
  int adj[N][N];
  int det;

} A;

struct data{
  int sign,i,j;
};

void display(int mat[][N])
{
  for(int i=0;i<N;i++)
  {
    for(int j=0;j<N;j++)
      printf("%d ",mat[i][j] );

    printf("\n" );

  }
}

void display_inv(float mat[][N])
{
  for(int i=0;i<N;i++)
  {
    for(int j=0;j<N;j++)
      printf("%f ",mat[i][j] );

    printf("\n" );

  }
}

// Function to get cofactor of A[p][q] in temp[][]. n is current
// dimension of A[][]
void getCofactor(int A[N][N], int temp[N][N], int p, int q, int n)
{
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                temp[i][j++] = A[row][col];

                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */

   int determinant(int A[N][N], int n)
   {
         int D = 0; // Initialize result
       //  Base case : if matrix contains single element
       if (n == 1)
           return A[0][0];

       int temp[N][N]; // To store cofactors

       int sign = 1;  // To store sign multiplier

        // Iterate for each element of first row
       for (int f = 0; f < n; f++)
       {
           // Getting Cofactor of A[0][f]
           getCofactor(A, temp, 0, f, n);
           D += sign * A[0][f] * determinant(temp, n - 1);

           // terms are to be added with alternate sign
           sign = -sign;
       }

       return D;
   }





void * ad( void * para )
{
  int temp[N][N];
  struct data *d = para;
  getCofactor(A.matrix, temp, d->i, d->j, N);
  A.adj[d->j][d->i] = (d->sign)*(determinant(temp, N-1));

}




// Function to get adjoint of A[N][N] in adj[N][N].
void  adjoint()
{
    if (N == 1)
    {
        A.adj[0][0] = 1;
        pthread_exit(0);
    }

    // temp is used to store cofactors of A[][]
    int sign = 1;

    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            struct data d;

            d.sign = ((i+j)%2==0)? 1: -1;
            d.i=i;
            d.j=j;

            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);

            pthread_create(&tid,&attr,ad,&d);
            pthread_join(tid,NULL);

        }
    }
}




// Function to calculate and store inverse, returns false if
// matrix is singular
int inverse()
{
    // Find determinant of A[][]

    if (A.det == 0)
    {
        printf( "Singular matrix, can't find its inverse");
        return 0;
    }


    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            A.inv[i][j] = A.adj[i][j]*1.0/(A.det);

    return 1;
}


// Driver program
int main()
{


    srand(time(NULL));

    for(int i=0;i<N;i++)
      for(int j=0;j<N;j++)
        A.matrix[i][j]=rand()%5;

  /*   A.matrix = { {5, -2, 2, 7},
                     {1, 0, 0, 3},
                     {-3, 1, 5, 0},
                     {3, -1, -9, 4}};
*/

    printf( "Input matrix is :\n");
    display(A.matrix);

      printf( "\nThe Adjoint is :\n");
      adjoint();

      display(A.adj);
      A.det=0;



      A.det = determinant(A.matrix, N);
      printf("Determinant : %d\n",A.det );
      printf( "\nThe Inverse is :\n");
      if (inverse(A.inv,A.det,A.adj))
          display_inv(A.inv);





    return 0;
}
