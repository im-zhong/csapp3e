// 2021/10/31
// zhangzhong
// Practice Problem 3.25

long loop_while2(long a, long b)
{
    long result = b;
    while (b > 0)
    {
        result *= a;
        b -= a;
    }
    return result;
}
