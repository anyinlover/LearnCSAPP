#include <stdio.h>

/* print Fahrenheit-Celsious table
    for fahr = 0, 20, ..., 300 */

void print_temp(int lower, int upper, int step)
{
    int fahr, celsius;
    fahr = lower;
    while (fahr <= upper) {
        celsius = 5 * (fahr - 32) / 9;
        printf("%d\t%d\n", fahr, celsius);
        fahr = fahr + step;
    }
}
int main()
{
    int lower, upper, step;
    
    lower = 0;      /* lower limit of temperature scale */
    upper = 300;    /* upper limit */
    step = 20;      /* step size */

    print_temp(lower, upper, step);
}
