#include <stdio.h>
#define MAXLINE 1000

int in = 0; /* 0: normal, 1: in comment, 2: in quoto, 3: in char, 4: in escape */
int mygetline(char line[], int maxline);
void unmatch(char line[], int count[]);

int main()
{
    int i;
    char line[MAXLINE];
    int count[3]; /* 0: (), 1: [], 2: {} */
    for (i = 0; i < 3; i++) {
        count[i] = 0;
    }

    while (mygetline(line, MAXLINE) > 0) {
        unmatch(line, count);
    }

    for (i = 0; i < 3; i++) {
        if (count[i] != 0) {
            printf("Index%d have %d unmatchs (0:parentheses, 1:brackets, 2:braces)\n", i, count[i]);
        }
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

void unmatch(char s[], int count[])
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
                } else if (s[i] == '\\') {
                    in = 4;
                } else if (s[i] == '(') {
                    ++count[0];
                } else if (s[i] == ')') {
                    --count[0];
                } else if (s[i] == '[') {
                    ++count[1];
                } else if (s[i] == ']') {
                    --count[1];
                } else if (s[i] == '{') {
                    ++count[2];
                } else if (s[i] == '}') {
                    --count[2];
                }
                ++i;
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
            ++i;
        } else if (in == 3) {
            if (s[i] == '\'') {
                in = 0;
            }
            ++i;
        } else if (in == 4) {
            in = 0;
            ++i;
        }
    }
}

