#include <stdio.h>
#define MAXLINE 1000

int mygetline(char line[], int maxline);
void reverse(char line[], int len);

int main()
{
    int len;
    char line[MAXLINE];

    while ((len = mygetline(line, MAXLINE)) > 0) {
        reverse(line, len);
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

void reverse(char s[], int len)
{
    char tmp;
    int i, j;
    i = 0;
    j = len - 1;
    if (s[j] == '\n')
        --j;
    while (i < j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        ++i;
        --j;
    }
}
