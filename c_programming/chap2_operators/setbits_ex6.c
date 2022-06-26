#include <stdio.h>

unsigned setbits(const unsigned x, const int p, const int n, const unsigned y);

int main()
{
    unsigned x, y;
    int p, n;
    while (scanf("%u %u %d %d", &x, &y, &p, &n) == 4) {
        x = setbits(x, p, n, y);
        printf("%u\n", x);
    }
    return 0;

}

unsigned setbits(const unsigned x, const int p, const int n, const unsigned y)
{
    unsigned nbits = ~(~0 << n);
    return (x & ~(nbits << (p - n + 1))) | ((y & nbits) << (p - n + 1));
}
