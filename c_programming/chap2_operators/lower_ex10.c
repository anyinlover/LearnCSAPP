#include <stdio.h>

int lower(int x);

int main()
{
    char x;
    int y;
    while (scanf("%c", &x) == 1) {
        y = lower(x);
        printf("%c\n", y);
    }
    return 0;

}

int lower(int c)
{
    return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}
