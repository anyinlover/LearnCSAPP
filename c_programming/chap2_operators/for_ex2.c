#include <stdio.h>
#define MAXLINE 1000

int mygetline(char line[], int maxline);

int main()
{
    char line[MAXLINE];

    while (mygetline(line, MAXLINE) > 0)
        printf("%s", line);
    
    return 0;
}

int mygetline(char s[], int lim)
{
    int c, i, finish;
    finish = 0;
    for (i = 0; !finish; i++)
        if ((c = getchar()) == '\n')
            finish = 1;
        else if (c == EOF)
            finish = 1;
        else if (i >= lim - 1)
            finish = 1;
        else
            s[i] = c;

    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    s[i] = '\0';

    return i;
}
