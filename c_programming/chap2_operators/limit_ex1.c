#include <stdio.h>
#include <limits.h>
#include <float.h>

int main()
{
    printf("signed char, max: %hhd, min: %hhd\n", SCHAR_MAX, SCHAR_MIN);
    printf("unsinged char, max: %hhu, min: 0\n", UCHAR_MAX);
    printf("signed short, max: %hd, min: %hd\n", SHRT_MAX, SHRT_MIN);
    printf("unsigned short, max: %hu, min: 0\n", USHRT_MAX);
    printf("signed int, max: %d, min: %d\n", INT_MAX, INT_MIN);
    printf("unsinged int, max: %u, min: 0\n", UINT_MAX);
    printf("signed long, max: %ld, min: %ld\n", LONG_MAX, LONG_MIN);
    printf("unsinged long, max: %lu, min: 0\n", ULONG_MAX);
    printf("float, max: %e, min: %e\n", FLT_MAX, -FLT_MAX);
    printf("double, max: %e, min: %e\n", DBL_MAX, -DBL_MAX);
}