#include <stdio.h>

int main()
{
    int c, oc;
    oc = EOF;
    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (oc != ' ' && oc != '\n' && oc != '\t' && oc != EOF)
                putchar('\n');
        } else {
            putchar(c);
        }
        oc = c;
    }
}