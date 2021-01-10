int mySqrt(int x)
{
    int l = 0;
    int h = 65535; //2^16 - 1 = 65535
    unsigned int m = h;

    while (l < h)
    {
        m = (l + h + 1) / 2;

        if (m * m == x)
        {
            return m;
        }
        else if (m * m > x)
        {
            h = m - 1;
        }
        else
        {
            l = m;
        }
    }
    return h;
}

int main()
{
    mySqrt(2);
    return 0;
}