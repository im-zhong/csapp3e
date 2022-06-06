// 2021/10/28
// zhangzhong
// Practice Problem 3.18

// long goto_test(long x, long y, long z)
// {
    // long val = x + y + z;
    // if (x >= -3)
        // goto L2;
    // if (y >= z)
        // goto L3;
    // val = x * y;
    // return val;
// L3:
    // val = y * z;
    // return val;
// L2:
    // if (x <= 2)
        // goto L4;
    // val = x * z;
// L4:
    // return val;
// }

long test(long x, long y, long z)
{
    long val = x + y + z;
    if (x < -3)
    {
        if (y < z)
            val = x * y;
        else
            val = y * z;
    }
    else if (x > 2)
        val = x * z;
    return val;
}
