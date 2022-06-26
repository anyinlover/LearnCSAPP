#include <stdio.h>

unsigned rightrot(const unsigned x, const int p);

int main()
{
    unsigned x;
    int n;
    while (scanf("%u %d", &x, &n) == 2) {
        x = rightrot(x, n);
        printf("%u\n", x);
    }
    return 0;

}

unsigned rightrot(const unsigned x, const int n)
{
    int size = sizeof(x) * 8;
    unsigned nbits = ~(~0 << n);
    return (x >> n) | ((x & nbits) << (size - n));
}
