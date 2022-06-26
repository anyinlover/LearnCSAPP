#include <stdio.h>
#define MAXLINE 1000

int mygetline(char s1[], char s2[], const int lim);
int any(const char s1[], const char s2[]);

int main()
{
    int i;
    char s1[MAXLINE];
    char s2[MAXLINE];
    while (mygetline(s1, s2, MAXLINE) > 0) {
        i = any(s1, s2);
        printf("%d\n", i);
    }
    return 0;

}

int mygetline(char s1[], char s2[], const int lim)
{
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s1[i] = c;

    if (c == '\n') {
        s1[i] = '\0';
    }

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s2[i] = c;

    s2[i] = '\0';
    return i;
}

int any(const char s1[], const char s2[])
{
    char count[128];
    int i;
    for (i = 0; i < 128; i++)
        count[i] = 0;
    i = 0;
    while (s2[i] != '\0') {
        count[s2[i++] - '\0'] = 1;
    }
    for (i = 0; s1[i] != '\0'; i++) {
        if (count[s1[i] - '\0'])
            return i;
    }
    return -1;
}
