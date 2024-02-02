#include <stdio.h>

int main(void)
{
    printf("%ld\n", __STDC_VERSION__);

    printf("Size of char:  %lu\n", sizeof(char));
    printf("Size of short: %lu\n", sizeof(short));
    printf("Size of int:   %lu\n", sizeof(int));
    printf("Size of long:  %lu\n", sizeof(long));
    printf("Size of long long: %lu\n", sizeof(long));

    printf("Size_t: %lu\n", sizeof(size_t));
}