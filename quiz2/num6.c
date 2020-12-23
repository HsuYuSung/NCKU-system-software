#include <stdio.h>
#include <stdlib.h>


int singleNumber(int *nums, int numsSize)
{
    int lower = 0, higher = 0;
    for (int i = 0; i < numsSize; i++) {
        lower ^= nums[i];
        printf("%d, lower^num      = %d\n", i, lower);
        lower &= ~higher;
        printf("%d, lower&~higher  = %d\n", i, lower);
        higher ^= nums[i];
        printf("%d, higher^num     = %d\n", i, higher);
        higher &= ~lower;
        printf("%d, higher&~lower  = %d\n", i, higher);
    }
    return lower;
}



int main()
{
    int num[] = {2, 2, 7, 2};
    int ans;
    ans = singleNumber(num, 4);
    return 0;
}

