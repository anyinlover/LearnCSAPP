#include <stdio.h>
#define MAXLINE 1000
#define N 8

int mygetline(char line[], int maxline);
void detab(char line[], char newline[]);

int main()
{
    char line[MAXLINE];
    char newline[MAXLINE];

    while (mygetline(line, MAXLINE) > 0) {
        detab(line, newline);
        printf("%s", newline);
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

void detab(char s[], char ns[])
{
    int i, j, k, q;
    i = j = k = 0;
    while (s[i] != '\0') {
        if (s[i] == '\t') {
            for(q = 0; q < N - k % N; ++q) {
                ns[j] = ' ';
                ++j;
            }
            k = 0;
        } else {
            ns[j] = s[i];
            ++j;
            ++k;
        }
        ++i;
    }
    ns[j] = '\0';
}

