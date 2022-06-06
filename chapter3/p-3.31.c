// 2021/10/31
// zhangzhong
// Practice Problem 3.31

void switcher(long a, long b, long c, long *dest)
{
    long val;
    switch(a)
    {
        case 5:
            c = b ^ 15;
            // fall through
        case 0:
            val = c + 112;
            break;
        case 2:;
        case 7:
            val = (b + c) * 4;
            break;
        case 4:
            val = a;
            break;
        default:
            val = b;
    }
    *dest = val;
}
