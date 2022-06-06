// 2021/10/31
// zhangzhong
// Practice Problem 3.26

long fun_a(unsigned long x)
{
    long val = 0;
    while (x != 0)
    {
        val ^= x;
        x >>= 1;
    }
    return val & 1;
}
