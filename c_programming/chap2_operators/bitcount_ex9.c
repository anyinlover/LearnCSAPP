#include <stdio.h>

int bitcount(unsigned x);

int main()
{
    unsigned x;
    int count;
    while (scanf("%u", &x) == 1) {
        count = bitcount(x);
        printf("%d\n", count);
    }
    return 0;

}

int bitcount(unsigned x)
{
    int b = 0;
    while (x != 0) {
        x &= (x - 1);
        b++;
    }
    return b;
}

