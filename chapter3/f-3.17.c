// 2021/10/31
// zhangzhong
// pipeling and branch prediction

long absdiff(long x, long y)
{
    long result;
    if (x < y)
        result = y - x;
    else
        result = x - y;
    return result;
}
