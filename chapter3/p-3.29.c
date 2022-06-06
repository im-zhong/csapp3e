// 2021/10/31
// zhangzhong
// Practice Problem 3.29

long fun()
{
    long sum = 0;
    long i;
    for (i = 0; i < 10; ++i)
    {
        if (i & 1)
            continue;
        sum += i;
    }
    return sum;
}
