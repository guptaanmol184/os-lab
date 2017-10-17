#include<stdio.h>

int main()
{
    char c=' ';
    printf("blank: %d;", c);
    printf("\0: %d;", '\0');
    printf("\n: %d;", '\n');
    printf("NULL: %d;", NULL);
    printf("EOF: %d;", EOF);
}
