#include <stdio.h>
#define CHARLEN 128

int main()
{
    int c, i, j, l;
    int charfreqs[CHARLEN];

    for (i = 0; i < CHARLEN; ++i)
        charfreqs[i] = 0;

    l = 0;
    while ((c = getchar()) != EOF) {
        ++charfreqs[c];
    }

    for (i = 0; i < CHARLEN; ++i) {
        printf("%c ", i);
        for (j = 0; j < charfreqs[i]; ++j)
            putchar('*');
        putchar('\n');
    }
}