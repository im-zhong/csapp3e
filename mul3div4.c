// 2021/10/14
// zhangzhong
// 2.79

#include <assert.h>
#include <limits.h>

// 3 * x / 4
int mul3div4(int x)
{
   x += (x << 1);
   (INT_MIN & x) && (x += (3));
   return x >> 2;
}

int main()
{
    assert(mul3div4(32) == 3 * 32 / 4);
    assert(mul3div4(-32) == 3 * -32 / 4);
}
