#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROCESS_COUNT 3
#define RESOURCE_TYPE_COUNT 4

// input matrices
int resource [RESOURCE_TYPE_COUNT];
int max [PROCESS_COUNT][RESOURCE_TYPE_COUNT];
int allocation [PROCESS_COUNT][RESOURCE_TYPE_COUNT];

// others
int available [RESOURCE_TYPE_COUNT];
int finish[PROCESS_COUNT] = {0};
int need [PROCESS_COUNT][RESOURCE_TYPE_COUNT];

int get_available(void);
int compare_array(int a[RESOURCE_TYPE_COUNT], char *s, int b[RESOURCE_TYPE_COUNT]);
void print1d(int a[RESOURCE_TYPE_COUNT]);
void print2d(int a[PROCESS_COUNT][RESOURCE_TYPE_COUNT]);
void calc_need();
int run_bankers();

int main()
{
    printf("Total number of processs: %d\n", PROCESS_COUNT);
    printf("Total number of resource types: %d\n", RESOURCE_TYPE_COUNT);

    printf("Enter resource count: ");
    for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
        scanf("%d", &resource[i]); 

    printf("Enter max matrix:\n");
    for(int i=0; i<PROCESS_COUNT; i++)
        for(int j=0; j<RESOURCE_TYPE_COUNT; j++)
            scanf("%d", &max[i][j]);

    printf("Enter allocation matrix:\n");
    for(int i=0; i<PROCESS_COUNT; i++)
        for(int j=0; j<RESOURCE_TYPE_COUNT; j++)
            scanf("%d", &allocation[i][j]);

    if(get_available() == 0)
    {
        printf("More resources are in use that available. INCORRECT INPUT!\n");
        exit(EXIT_SUCCESS);
    }
    printf("Available resource count: ");
    print1d(available);

    // bankers algo
    int state = run_bankers();
    
    if(state == 0)
        printf("\nThe system is in UNSAFE state.\n");
    else
        printf("\nThe system is in SAFE state.\n");
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
//    printf("compare_array: ");
    if(strcmp(s, ">=") == 0)
    {
        for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
        {
//            printf("a[%d]: %d, b[%d]: %d\n", i, a[i], i, b[i]);
            if(a[i]<b[i])
            {
                return 0;
            }
        }
    }
    
    return 1;
}

void print1d(int a[RESOURCE_TYPE_COUNT])
{
    for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
        printf("%d ", a[i]);
    printf("\n");
}

void print2d(int a[PROCESS_COUNT][RESOURCE_TYPE_COUNT])
{
    printf("\n");
    for( int i=0; i<PROCESS_COUNT; i++)
    {
        for(int j=0; j<RESOURCE_TYPE_COUNT; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
}

void calc_need()
{
    for(int i=0; i<PROCESS_COUNT; i++)
        for(int j=0; j<RESOURCE_TYPE_COUNT; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

// get a row such that it is not finished and
// it's need >= available
int get_row()
{

    for(int i=0; i<PROCESS_COUNT; i++)
        if(finish[i]!=1 && compare_array(available, ">=", need[i]))
            return i;

    return -1;
}

// check if everything is finished
int check_finish()
{
    for(int i=0; i<PROCESS_COUNT; i++)
        if(finish[i]==0)
            return 0;
    return 1;
}

int run_bankers()
{
    int is_finish = check_finish();
    int row;
    calc_need();
    get_available();
//    printf("need");
//    print2d(need);

    while(is_finish!=1)
    {
        row = get_row();
        is_finish = check_finish();
//        printf("finish: %d\n", is_finish);
//        printf("row: %d\n", row);

        if((is_finish ==0) && (row == -1))
            return 0;

        finish[row] = 1;

        // deallocate resources
        for(int i=0; i<RESOURCE_TYPE_COUNT; i++)
        {
            available[i] = available[i]+allocation[row][i];
            allocation[row][i] = 0;
        }
//        print1d(available);
//        print2d(allocation);

        // recalculate need
        calc_need();
    }

    return 1;
}
