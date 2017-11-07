#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define DEPOSIT 1
#define WITHDRAWL -1
#define MaxCustomers 10
#define NoOfCounters 3

int NextToken=1, AvailableToken=0;

float GlobalBalance=10000000.0;
int Counter[NoOfCounters];
int CustSlot[MaxCustomers], front=-1, rear=-1;

// customer details
struct Customer
{
  float Balance;
  int AccountNo;
  int TokenNo;
  int Task;
  float Amount;
}customer[MaxCustomers];

sem_t  Mutex, tok, Cust,balance;

/*
Mutex: Counter[]
tok:  AvailableToken,NextToken
Cust :  enqueue, dequeue(CustSlot, front,end), customer[]
balance:  GlobalBalance
*/

void enqueue(int a);
int  dequeue();
void rand_sleep();

void * InitCustomer();
void * counter( void * arg);
void   GenerateToken( int  arg);
void   UpdateGlobal(float arg);
void   AllocateToken(int arg);

int main()
{
  sem_init(&Mutex,0,1);
  sem_init(&tok,0,1);
  sem_init(&Cust,0,NoOfCounters);
  sem_init(&balance,0,1);

  pthread_t counter_thread[NoOfCounters],customer_thread;
  pthread_attr_t attr;
  pthread_attr_init(&attr);


  for(int i=0;i<MaxCustomers;i++)
    enqueue(i);

  for(int i=0;i<NoOfCounters;i++)
  {
    pthread_create(&counter_thread[i],&attr,counter,(void *)i);

  }

  pthread_create(&customer_thread,&attr,InitCustomer,NULL);

  pthread_join(customer_thread,NULL);

  for(int i=0;i<NoOfCounters;i++)
  {
    pthread_join(counter_thread[i],NULL);
  }

  return 0;
}

// Function to create new customers
void * InitCustomer()
{

  printf("%s\n", "customers");
  while(1)
  {
    sem_wait(&Cust);
    int n=dequeue();

    srand(time(NULL));
    if(n>=0)
    {

      printf("\n\n New Customer Enters:  ");

      customer[n].AccountNo=(rand()%10000000);
      customer[n].Task=rand()%2;
      if(customer[n].Task==0)
        customer[n].Task=-1;
      customer[n].Amount= (rand()%1000) * 1.0;
      customer[n].Balance= (rand()%100000) * 1.0;
      GenerateToken(n);

      printf("\n TokenNo : %d\n AccountNo: %d",customer[n].TokenNo,customer[n].AccountNo);

      if(customer[n].Task == DEPOSIT)
        printf("\n Task : DEPOSIT\n Amount: %f\n Current Balance: %f",customer[n].Amount,customer[n].Balance);

      else
        printf("\n Task : WITHDRAWL\n Amount: %f\n Current Balance: %f",customer[n].Amount,customer[n].Balance);

      printf("\n" );

    }
    sem_post(&Cust);
    sleep(1);
  }
}


// Allocates token to counters
void  AllocateToken(int Num)
{
  sem_wait(&tok);
  if(AvailableToken==NextToken)
    Counter[Num]=-1;
  else
  {
    Counter[Num] = AvailableToken;
    AvailableToken++;
  }
  sem_post(&tok);

}




// counters
void * counter( void * arg)
{
  sleep(4);
  int num = (int)arg;

  while (1)
  {

    sem_wait(&Mutex);
    AllocateToken(num);
    sem_post(&Mutex);
    if(Counter[num]>0)
      printf("\n\t[counter%d]Allocated Token = %d\n\n", num,Counter[num]);
    if(Counter[num]>0)
    {
      for(int i=0;i<MaxCustomers;i++)
      {
        if( customer[i].TokenNo == Counter[num])
          {

            sem_wait(&Cust);
            float amt= customer[i].Task * customer[i].Amount;
            sem_post(&Cust);
            rand_sleep();
            sem_wait(&Cust);
            customer[i].Balance+=amt;
            printf("\n\t[counter %d] Transaction completed for customer %d. Available Balance: %f",num,customer[i].TokenNo,customer[i].Balance);
            printf("\n\tAmount updated: %f\n",amt);
            UpdateGlobal(amt);

            enqueue(i);
            sem_post(&Cust);

          }
      }
    }


    /// a function to free the cust id will come below


  }


}


void GenerateToken(int cust)
{
  sem_wait(&tok);
  customer[cust].TokenNo=NextToken;

  NextToken++;
  sem_post(&tok);
}

void UpdateGlobal(float add)
{
  sem_wait(&balance);
  GlobalBalance+= add;
  printf("\tGlobal Account Updated! New bal: %f \n\n",GlobalBalance);
  sem_post(&balance);
}

int dequeue()
{
  if (front==-1)
  {
  //  printf("\n\n No Customer Exists!!");
    return -1;
  }

  else
  {
    int tNo=CustSlot[front];
    if(front==rear)
      front = rear = -1;
    else
      front = (front+1)%MaxCustomers;

    return tNo;
  }
}


void enqueue(int data)
{
  if(front==-1)
    front++;

  rear=++rear % MaxCustomers;
  CustSlot[rear]=data;
}

void rand_sleep()
{
  srand(time(NULL));
  int t=rand()%5+1;
  sleep(t);
}
