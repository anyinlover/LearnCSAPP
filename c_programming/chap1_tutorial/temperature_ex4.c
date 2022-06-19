#include <stdio.h>

/* print Fahrenheit-Celsious table
    for fahr = 0, 20, ..., 300 */

main()
{
    int fahr, celsius;
    int lower, upper, step;
    
    lower = 0;      /* lower limit of temperature scale */
    upper = 300;    /* upper limit */
    step = 20;      /* step size */

    celsius = lower;
    while (celsius <= upper) {
        fahr = 9 * celsius / 5 + 32;
        printf("%d\t%d\n",  celsius, fahr);
        celsius = celsius + step;
    }
}