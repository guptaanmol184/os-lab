#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

// Fibonacci program
int fib(int n)
{
   if (n <= 1)
      return n;
   return fib(n-1) + fib(n-2);
}

int main()
{
    int term;
    printf("Enter term: ");
    scanf("%d", &term);

    pid_t pid = fork();

    if(pid<0)
        printf("Fork failed...");
    else if(pid==0)
    {
        printf("Calating %d th fibonacci term", term);
        printf("%d th fibonacci term is %d", term, fib(term));
    }
    else
        wait(NULL);
}


