// 2021/10/28
// zhangzhong
// Practice Problem 3.16

void cond(long a, long *p)
{
    if (p && a > *p)
        *p = a;
}

void cond_goto(long a, long *p)
{
    if (!p)
        goto L1;
    if (*p >= a)
        goto L1;
    *p = a;
L1:;
}
