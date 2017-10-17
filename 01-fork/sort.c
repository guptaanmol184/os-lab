#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int cmpfuc_asc (const void *a, const void *b)
{
    return( *(int*)a - *(int*)b);
}

int cmpfuc_dsc (const void *a, const void *b)
{
    return( *(int*)b - *(int*)a);
}

int main()
{
    int n;
    printf("Total number of items in array: ");
    scanf("%d", &n);

    int a[n];
    printf("Enter array: \n");
    for( int i=0; i< n; i++)
    {
        scanf("%d", &a[i]);
    }

    pid_t pid = fork();

    if(pid<0)
        printf("Fork failed");
    else if(pid == 0)
    {
        qsort(a, n, sizeof(int), cmpfuc_asc);

        printf("Ascending - child: ");
        for( int i=0; i<n; i++)
            printf("%d ", a[i]);
        printf("\n");
    }
    else
    {
        wait(NULL);
        qsort(a, n, sizeof(int), cmpfuc_dsc);

        printf("Descending - parent: ");
        for( int i=0; i<n; i++)
            printf("%d ", a[i]);
        printf("\n");
    }
}


