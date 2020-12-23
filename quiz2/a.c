#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


const uint32_t D = 0xFFFFFFFF;
#define M ((uint64_t)(UINT64_C(0xFFFFFFFFFFFFFFFF) / D + 1))

bool divisible(uint32_t n)
{
    return n * M <= (M-1);
}


int main()
{
    printf("%llu\n", 19 * M);
    printf("%llu\n", (M>>1));
    // printf("%d\n", divisible(3));
    uint32_t num;
    while (1) {
        scanf("%x", &num);
        printf("divisible  %d\n", divisible(num));
    }

    return 0;
}