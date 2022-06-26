#include <stdio.h>
#define MAXLINE 1000

int in = 0; /* 0: normal, 1: in comment, 2: in quoto, 3: in char */
int mygetline(char line[], int maxline);
void rmcomment(char line[], char newline[]);

int main()
{
    char line[MAXLINE];
    char newline[MAXLINE];

    while (mygetline(line, MAXLINE) > 0) {
        rmcomment(line, newline);
        printf("%s", newline);
    }
    
    return 0;
}

int mygetline(char s[], int lim)
{
    int i, c;
    i = c = 0;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;

    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    s[i] = '\0';

    return i;
}

void rmcomment(char s[], char ns[])
{
    int i, j;
    i = j = 0;
    while (s[i] != '\0') {
        if (in == 0) {
            if (s[i] == '/' && s[i+1] == '*') {
                in = 1;
                i += 2;
            } else {
                if (s[i] == '"') {
                    in = 2;
                } else if (s[i] == '\'') {
                    in = 3;
                }
                ns[j++] = s[i++];
            }
        } else if (in == 1) {
            if (s[i] == '*' && s[i+1] == '/') {
                in = 0;
                i += 2;
            } else {
                ++i;
            }
        } else if (in == 2) {
            if (s[i] == '"') {
                in = 0;
            }
            ns[j++] = s[i++];
        } else if (in == 3) {
            if (s[i] == '\'') {
                in = 0;
            }
            ns[j++] = s[i++];
        }
    }
    ns[j] = '\0';
}
