#include <stdio.h>

unsigned invert(const unsigned x, const int p, const int n);

int main()
{
    unsigned x;
    int p, n;
    while (scanf("%u %d %d", &x, &p, &n) == 3) {
        x = invert(x, p, n);
        printf("%u\n", x);
    }
    return 0;

}

unsigned invert(const unsigned x, const int p, const int n)
{
    unsigned y = ~(x >> (p+1-n));
    unsigned nbits = ~(~0 << n);
    return (x & ~(nbits << (p - n + 1))) | ((y & nbits) << (p - n + 1));
}
