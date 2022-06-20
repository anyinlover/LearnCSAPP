#include <stdio.h>

#define MAXLEN 10

int main()
{
    int c, i, j, l;
    int wordlens[MAXLEN];

    for (i = 0; i < MAXLEN; ++i)
        wordlens[i] = 0;

    l = 0;
    while ((c = getchar()) != EOF) {
        if (c != ' ' && c != '\t' && c != '\n')
            ++l;
        else if (l > 0) {
            if (l < MAXLEN)
                ++wordlens[l-1];
            l = 0;
        }
    }

    for (i = 0; i < MAXLEN; ++i) {
        printf("%d ", i+1);
        for (j = 0; j < wordlens[i]; ++j)
            putchar('*');
        putchar('\n');
    }
}