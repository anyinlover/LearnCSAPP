#include <stdio.h>

int main()
{
    int c, oc;
    oc = EOF;
    while ((c = getchar()) != EOF) {
        if (c != ' ' || (c == ' ' && oc != ' '))
            putchar(c);
        oc = c;
    }
}