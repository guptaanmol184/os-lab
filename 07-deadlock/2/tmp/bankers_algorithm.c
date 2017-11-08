#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

int NoOfResources,NoOfProcesses;

int generate_sequence(int needs[][NoOfResources],int avail[],int allocated[][NoOfResources]);

int main(int argc , char ** argv)
{
    srand(time(NULL));

    if(argc<3)
    {
        printf("\n  Usage : ./a.out <NoOfResources> <NoOfProcesses>\n");
        exit(0);
    }

    NoOfResources = atoi(argv[1]);
    NoOfProcesses = atoi(argv[2]);

    int resources[NoOfResources];
    int needs[NoOfProcesses][NoOfResources];
    int avail[NoOfResources];
    int allocated[NoOfProcesses][NoOfResources];

    for(int i=0;i<NoOfProcesses;i++)
    {
        for(int j=0;j<NoOfResources;j++)
        {
            needs[i][j] = rand()%10+5;
            allocated[i][j] = rand()%5+4;
        }
    }

    for(int i=0;i<NoOfResources;i++)
    {
        resources[i]=0;
        for(int j=0; j<NoOfProcesses;j++)
            resources[i]+=needs[j][i];
    }

    for(int i=0;i<NoOfResources;i++)
    {
        int a=rand()%7+7;
        resources[i]+=a;
        avail[i]=a;
    }

    printf("Generated allocated matrix ( col -> resources):\n\n");
    for(int i=0;i< NoOfProcesses; i++)
    {
        printf("  P%d ", i);
        for(int j=0;j< NoOfResources; j++)
            printf(" %3d",allocated[i][j]);
        printf("\n");
    }


    printf("Generated needs matrix (col -> resources):\n\n");
    for(int i=0;i< NoOfProcesses; i++)
    {
        printf("  P%d ", i);
        for(int j=0;j< NoOfResources; j++)
            printf(" %3d",needs[i][j]);
        printf("\n");
    }


    printf("Resources: ");
    for(int j=0;j< NoOfResources; j++)
        printf(" %3d",resources[j]);
    printf("\n");

    if(!generate_sequence(needs,avail,allocated))
        printf("No sequence!\n\n");

    return 0;
}

int generate_sequence(int needs[][NoOfResources],int avail[],int allocated[][NoOfResources])
{
    int count=0;
    int LastInSequence=NoOfProcesses-1;

    printf("Sequence: ");
    while(count<NoOfProcesses)
    {
        for(int i=0;i<NoOfProcesses;i++)
        {
            int flag=1;
            for(int j=0;j<NoOfResources;j++)
                if(needs[i][j]> avail[j])
                {
                    flag=0;
                    //  break;
                }

            if(LastInSequence==i)
                return 0;

            if(flag==1)
            {
                printf(" P%d",i);
                LastInSequence =i;
                count++;
                for(int j=0;j<NoOfResources;j++)
                {
                    avail[j]+=needs[i][j];
                    needs[i][j]=10000;
                }
            }
        }// end outer for
    }// end while

    printf("\n\n");
    return 1;

}
