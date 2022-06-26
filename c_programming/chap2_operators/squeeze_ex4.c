#include <stdio.h>
#define MAXLINE 1000

int mygetline(char s1[], char s2[], int lim);
void squeeze(char s1[], const char s2[]);

int main()
{
    char s1[MAXLINE];
    char s2[MAXLINE];
    while (mygetline(s1, s2, MAXLINE) > 0) {
        squeeze(s1, s2);
        printf("%s\n", s1);
    }
    return 0;

}

int mygetline(char s1[], char s2[], int lim)
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

void squeeze(char s1[], const char s2[])
{
    char count[128];
    int i, j;
    for (i = 0; i < 128; i++)
        count[i] = 0;
    i = 0;
    while (s2[i] != '\0') {
        count[s2[i++] - '\0'] = 1;
    }
    for (i = j = 0; s1[i] != '\0'; i++) {
        if (!count[s1[i] - '\0'])
            s1[j++] = s1[i];
    }
    s1[j] = '\0';
}