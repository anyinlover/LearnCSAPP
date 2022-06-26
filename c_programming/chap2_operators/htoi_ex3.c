#include <stdio.h>
#include <ctype.h>
#define MAXLINE 1000

unsigned long htoi(char s[]);
int mygetline(char s[], int lim);

int main()
{
    unsigned long result;
    char line[MAXLINE];
    while (mygetline(line, MAXLINE) > 0) {
        result = htoi(line);
        printf("%s convert to %lu\n", line, result);
    }
    return 0;
}

int mygetline(char s[], int lim)
{
    int c, i;
    i = 0;
    while ((c = getchar()) != EOF && c!='\n') {
        if (i < lim - 1)
            s[i] = c;
        ++i;
    }
    if (i >= lim)
        s[lim - 1] = '\0';
    else
        s[i] = '\0';
    
    return i;
}

unsigned long htoi(char s[])
{
    int i = 0;
    unsigned long r = 0;
    if (s[i] == '0' && (s[i+1] == 'x' || s[i+1] == 'X'))
        i = 2;
    while (s[i] != '\0') {
        r *= 16;
        if (isdigit(s[i])) {
            r += s[i] - '0';
        } else if (islower(s[i])) {
            r += s[i] - 'a' + 10;
        } else if (isupper(s[i])) {
            r += s[i] - 'A' + 10;
        }
        ++i;
    }
    return r;
}