// C++ program to find adjoint and inverse of a matrix
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define N 4 // matrix size

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
			// Copying into temporary matrix only those element
			// which are not in given row and column
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

	// Base case : if matrix contains single element
	if (n == 1)
		return A[0][0];

	int temp[N][N]; // To store cofactors

	int sign = 1; // To store sign multiplier

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

// Function to get adjoint of A[N][N] in adj[N][N].
void adjoint(int A[N][N],int adj[N][N])
{
	if (N == 1)
	{
		adj[0][0] = 1;
		return;
	}

	// temp is used to store cofactors of A[][]
	int sign = 1, temp[N][N];

	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
		{
			// Get cofactor of A[i][j]
			getCofactor(A, temp, i, j, N);

			// sign of adj[j][i] positive if sum of row
			// and column indexes is even.
			sign = ((i+j)%2==0)? 1: -1;

			// Interchanging rows and columns to get the
			// transpose of the cofactor matrix
			adj[j][i] = (sign)*(determinant(temp, N-1));
		}
	}
}

// Function to calculate and store inverse, returns false if
// matrix is singular
int inverse(int A[N][N], float inverse[N][N])
{
	// Find determinant of A[][]
	int det = determinant(A, N);
	if (det == 0)
	{
		printf("Singular matrix, can't find its inverse");
		return 0;
	}

	// Find adjoint
	int adj[N][N];
	adjoint(A, adj);

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)"
	for (int i=0; i<N; i++)
		for (int j=0; j<N; j++)
			inverse[i][j] = adj[i][j]/(float)det;

	return 1;
}

void display_int(int A[N][N])
{
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
            printf("%d ", A[i][j]);
        printf("\n");
	}
}
void display_float(float A[N][N])
{
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
            printf("%f ", A[i][j]);
        printf("\n");
	}
}

int main()
{
    pid_t pid;
	int A[N][N] = { {5, -2, 2, 7},
					{1, 0, 0, 3},
					{-3, 1, 5, 0},
					{3, -1, -9, 4}};
	int adj[N][N]; // To store adjoint of A[][]
	float inv[N][N]; // To store inverse of A[][]

	printf("Input matrix is :\n");
	display_int(A);

    pid = fork();

    switch(pid)
    {
        case -1:// error
            printf("Error forking");
        case 0: // child
            {
                // generate adjoint
                printf("\nThe Adjoint is :\n");
                adjoint(A, adj);
                display_int(adj);
            }
        case 1:
            {
                wait(NULL); // wait for child to generate the adjoint matrix
                printf("\nThe Inverse is :\n");
                if (inverse(A, inv))
                    display_float(inv);
            }
    }

	return 0;
}

