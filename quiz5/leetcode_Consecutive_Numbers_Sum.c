int consecutiveNumbersSum(int N)
{
    if (N < 1)
        return 0;
    int ret = 1;
    int x = N;
    for (int k = 2; k < sqrt(2 * x); k++)
    {
        if ((x - (k * (k - 1) / 2)) % k == 0)
            ret++;
    }
    return ret;
}