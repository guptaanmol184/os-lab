#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define M 3
#define K 3
#define N 3

int c[M][N];
int a[M][K] = {{1,0,0}, {0,1,0}, {0,0,1}};
int b[K][N] = {{1,2,1}, {2,1,3}, {1,2,3}};

void* runner(void *params);

struct matdata
{
    int row;
    int col;
};

int main()
{
    pthread_t tid[M*N];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for( int i=0; i<M; i++)
    {
        for( int j=0; j<N; j++)
        {
            struct matdata* data= (struct matdata*) malloc(sizeof(struct matdata));
            data->row = i;
            data->col = j;

            pthread_create(&tid[i*N+j], &attr, &runner, data);
        }
    }

    for( int i=0; i<M*N; i++)
        pthread_join(tid[i], NULL);

    printf("product matrix:\n");
    for(int i=0; i<M; i++)
    {
        for(int j=0;j<N; j++)
        {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }

    return 0;
}

void* runner(void *params)
{
    int sum=0;
    struct matdata *lmd = (struct matdata*) params;

    for( int z=0; z<K; z++)
    {
        sum+= a[lmd->row][z] * b[z][lmd->col];
        c[lmd->row][lmd->col] = sum;
    }

    pthread_exit(0);

}
