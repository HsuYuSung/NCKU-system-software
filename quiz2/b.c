#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


#define PACKED_BYTE(b) (((uint64_t)(b) & (0xff)) * 0x0101010101010101u)


int main()
{
    printf("%lx\n", 0x0101010101010101u);
    printf("%llu\n", PACKED_BYTE(0x10));

    printf("%d", (5 & 2) >> 1);
    return 0;

}