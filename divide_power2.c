// 2021/10/14
// zhangzhong
// 2.78

#include <limits.h>
#include <assert.h>

int divide_power2(int x, int k)
{
    (INT_MIN & x) && (x += (1 << k) - 1);
    return x >> k; 
}

int main()
{
    assert(divide_power2(12345, 3) == 12345 / (1 << 3));
}
