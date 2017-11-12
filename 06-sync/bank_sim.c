#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#define MIN_BAL 100

int Global_bal = 100000;	//Initial bank balance
int tr_id = 0;				//transaction id


typedef struct 				//Customer detail structure
{
	int acc_no;
	int bal;
} acc_details;


sem_t mutex;				//to update global balance
sem_t counter;				//to allocate counter

int choice()				//To choose credit or debit
{
	return (rand()%2);
}

//Function to credit amount into the account
void credit(acc_details *details)
{
	printf("Customer %d is waiting for counter\n", details->acc_no);
	sem_wait(&counter);

	int amt = rand()%10000;

	sem_wait(&mutex);

	int ctr;
	sem_getvalue(&counter,&ctr);
	details -> bal = details -> bal + amt;

	tr_id++;

	Global_bal = Global_bal + amt;

	printf("Customer %d credited %d rupees in counter %d\n",details->acc_no, amt, ctr );
	printf("Transaction id #%d\n",tr_id );
	printf("Customer %d's updated balance %d\n", details->acc_no,details->bal );
	printf("Total Bank balance = %d\n",Global_bal );

	sem_post(&mutex);
    sleep(rand()%3);

	sem_post(&counter);
	return;
}

void debit(acc_details *details)
{
	printf("Customer %d is waiting for counter\n", details->acc_no);
	sem_wait(&counter);

	int amt = rand()%10000;

	//If the debit amount causes the account to go below minimum balance 
	if(details -> bal - amt < MIN_BAL)
	{
		printf("Transaction not possible ... Not enough balance in account\n");
		sem_post(&counter);
		return;
	}

	sem_wait(&mutex);

	int ctr;
	sem_getvalue(&counter,&ctr);
	details -> bal = details -> bal - amt;


	tr_id++;

	Global_bal = Global_bal - amt;

	printf("Customer %d debited %d rupees in counter %d\n",details->acc_no, amt, ctr );
	printf("Transaction id #%d\n",tr_id );
	printf("Customer %d's updated balance %d\n", details->acc_no,details->bal );
	printf("Total Bank balance = %d\n",Global_bal );

	sem_post(&mutex);
    sleep(rand()%3);


	sem_post(&counter);
	return;
}

void *transaction(void *param)
{
	while(1)
	{
		acc_details *details = (acc_details *)param;
		printf("Customer %d is going to bank\n",details->acc_no);
		sleep(2);
		printf("Customer %d reached bank\n",details->acc_no );

		int ch = choice();

		if(ch == 0)
			credit(details);
		else
			debit(details);
		printf("Customer %d is leaving the bank\n",details -> acc_no );
	}

	pthread_exit(0);
}


int main()
{
	srand(time(NULL));

	int n;

	printf("Enter the number of customers\n");
	scanf("%d",&n);

	acc_details customers[n];

	pthread_t tid[n];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	//Initializing the customer details
	for(int i=0;i<n;i++)
	{
		customers[i].acc_no = i+1;
		customers[i].bal = 10000 + (rand()%1000);
	}

	//Initializing semaphores
	sem_init(&mutex,0,1);			//Global balance
	sem_init(&counter,0,3);			//Counter

	//Thread creation
	for(int i=0;i<n;i++)
	{
		pthread_create(&tid[i],&attr,transaction,&(customers[i]));
	}
	for(int i=0;i<n;i++)
	{
		pthread_join(tid[i],NULL);
	}
}
