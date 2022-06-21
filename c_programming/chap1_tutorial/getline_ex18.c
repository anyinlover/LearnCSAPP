#include <stdio.h>
#define MAXLINE 1000

int mygetline(char line[], int maxline);
int removetrail(char line[], int len);

int main()
{
    int len;
    char line[MAXLINE];

    while ((len = mygetline(line, MAXLINE)) > 0) {
        if (removetrail(line, len) > 0)
            printf("%s", line);
    }
    
    return 0;
}

int mygetline(char s[], int lim)
{
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;

    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    s[i] = '\0';

    return i;
}

int removetrail(char s[], int len)
{
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t'))
        --len;
    s[len] = '\0';

    return len;
}