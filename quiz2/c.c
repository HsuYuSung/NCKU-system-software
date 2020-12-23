#include <stdio.h>
#include <stdint.h>



int main()
{   
    u_int8_t a = 0x0;
    u_int8_t b = 0x1;
    int num = -3;
    printf("%u\n", a - b);
    printf("%d\n", num >> 31);
    return 0;
}