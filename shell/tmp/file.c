#include<stdio.h>
#include<string.h>

int main()
{
    FILE *fp;

    fp = fopen("history.txt", "a");

    char str[1000] = "";
    char c;

    while(c=getchar() != EOF)
    {
        scanf("%s", str);
        printf("%s", str);
        fwrite( str, strlen(str), 1, fp);
    }

    fclose(fp);

}
