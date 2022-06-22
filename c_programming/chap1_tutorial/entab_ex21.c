#include <stdio.h>
#define MAXLINE 1000
#define N 8

int mygetline(char line[], int maxline);
void entab(char line[], char newline[], int len);

int main()
{
    int len;
    char line[MAXLINE];
    char newline[MAXLINE];

    while ((len = mygetline(line, MAXLINE)) > 0) {
        entab(line, newline, len);
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

void entab(char s[], char ns[], int len)
{
    int i, j, k, q;
    i = N - 1;
    j = k = 0;
    while (i < len) {
        k = i;
        while (k > i - N && s[k] == ' ')
            --k;
        for (q = i - N + 1; q <= k; ++q) {
            ns[j] = s[q];
            ++j;
        }
        if (k < i) {
            ns[j] = '\t';
            ++j;
        }
        i += N;
    }
    for (q = i - N + 1; q < len; ++q) {
        ns[j] = s[q];
        ++j;
    }
    ns[j] = '\0';
}
