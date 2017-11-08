#include <stdio.h>

#define PROCESS_COUNT 5
#define RESOURCE_TYPE_COUNT 3

// matrices
int resource [RESOURCE_TYPE_COUNT];
int available [RESOURCE_TYPE_COUNT];
int max [PROCESS_COUNT][RESOURCE_TYPE_COUNT];
int allocation [PROCESS_COUNT][RESOURCE_TYPE_COUNT];

int get_available(void);
int compare_array(int a[RESOURCE_TYPE_COUNT], char *s, int b[RESOURCE_TYPE_COUNT]);

int main()
{
    printf("Total number of processs: %d\n", PROCESS_COUNT);
    printf("Total number of resource types: %d\n", RESOURCE_TYPE_COUNT);

    printf("Enter resource count: ");
    for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
       scanf("%d", &resource[i]); 

    printf("Enter max matrix:\n");
    for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
        for(int j=0; j<RESOURCE_TYPE_COUNT; j++)
            scanf("%d", &max[i][j]);

    printf("Enter allocation matrix:\n");
    for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
        for(int j=0; j<RESOURCE_TYPE_COUNT; j++)
            scanf("%d", &allocation[i][j]);

    int sanity = get_available();
    if(sanity == 0)
    {
        printf("The re");
    }


}

int get_available(void)
{
    int in_use[RESOURCE_TYPE_COUNT] = {0};
    for(int i=0; i<PROCESS_COUNT; i++)
        for(int j=0; j<RESOURCE_TYPE_COUNT; j++)
            in_use[j]+=allocation[i][j];

    // compare sum with resource matrix
    for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
        if(resource[i] < in_use[i])
            return 0;

    // get available
    for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
        available[i] = resource[i] - in_use[i];

    return 1;
}

int compare_array(int a[RESOURCE_TYPE_COUNT], char *s, int b[RESOURCE_TYPE_COUNT])
{

}
