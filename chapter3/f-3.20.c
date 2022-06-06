// 2021/10/31
// zhangzhong
// Figure 3.20

long fact_while(long n)
{
    long result = 1;
    while (n > 1)
    {
        result *= n;
        n = n - 1;
    }
    return result;
}
