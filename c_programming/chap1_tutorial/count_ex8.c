#include <stdio.h>

int main()
{
    int c;
    long nb, nt, nl;

    nb = nt = nl = 0;
    while ((c = getchar()) != EOF) {
        if (c == ' ')
            ++nb;
        else if (c == '\t')
            ++nt;
        else if (c == '\n')
            ++nl;
    }
    printf("blank num: %ld, tab num: %ld, newline num: %ld\n", nb, nt, nl);

    return 0;
}