#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

const uint32_t D = 2;
#define C 4 / 2 + 1
#define M ((uint64_t)(UINT64_C(0xFFFFFFFFFFFFFFFF) / (D) + 1))

// tool
uint64_t int2int(uint64_t k) {
    if (k == 0) return 0;
    if (k == 1) return 1;                       /* optional */
    return (k % 2) + 10 * int2int(k / 2);
}

bool divisible(uint32_t n)
{
    printf("M = %llu\n", M);
    printf("M+1   = %llu\n", M+1);
    printf("n * M = %llu\n", n * M);
    return n * M <= M-1;
}

/* compute (n mod d) given precomputed M */
uint32_t quickmod(uint32_t n)
{   
    printf("M * n = %llu\n", int2int(M * n));
    uint64_t quotient = ((__uint128_t) M * n) >> 64;
    printf("quotient = %llu\n", quotient);
    return n - quotient * D;
}

int main()
{
    uint8_t bi = 0b11111111;

    uint32_t ans = quickmod(14);
    printf("%d\n", bi/3);
    printf("%llu\n", int2int(85));
    printf("ans = %d\n", ans);
    printf("%d\n", 0b1111 / 0b11);
    bool b = divisible(1);
    printf("%d\n", b);
    // printf("M = %llu\n", M);
    // printf("M2bin = %llu\n", int2int(M));

    printf("%llu\n", UINT64_C(0xFFFFFFFFFFFFFFFF));
    return 0;
}