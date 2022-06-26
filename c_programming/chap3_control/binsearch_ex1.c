#include <stdio.h>
#define MAXLEN 1000

int binsearch(int x, int v[], int n);

int main()
{
    int x, n, i;
    int v[MAXLEN];
    scanf("%d", &x);
    scanf("%d", &n);
    for (i = 0; i < n; ++i)
        scanf("%d", &v[i]);

    printf("%d\n", binsearch(x, v, n));
    return 0;
}

int binsearch(int x, int v[], int n)
{
    int low, high, mid;
    low = 0;
    high = n - 1;
    while (low < high) {
        mid = (low + high) / 2;
        if (x > v[mid])
            low = mid + 1;
        else
            high = mid;
    }
    return v[low] == x ? low : -1;
}

