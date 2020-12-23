#include <stdlib.h>

int findDuplicate(int *nums, int numsSize)
{
    int res = 0;
    const size_t log_2 = 8 * sizeof(int) - __builtin_clz(numsSize);
    for (size_t i = 0; i < log_2; i++) {
        int bit = 1 << i;
        int c1 = 0, c2 = 0;
        for (size_t k = 0; k < numsSize; k++) {
            if (k & bit)
                ++c1;
            if (nums[k] & bit)
                ++c2;
        }
        if (c1 < c2)
            res += bit;
    }
    return res;
}

int main()
{}