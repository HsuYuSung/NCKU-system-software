int consecutiveNumbersSum(int N)
{
    if (N < 1)
        return 0;
    int ret = 1;
    int x = N;
    for (int i = 2; i < x; i++) {
        x += 1-i;
        if (x % i == 0)
            ret++;
    }
    return ret;
} 