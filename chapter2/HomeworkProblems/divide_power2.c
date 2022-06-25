// 2021/10/14
// zhangzhong
// Homework Problem 2.78

#include <limits.h>
#include <assert.h>
#include <stdio.h>

// divide by power of 2
// k: [0, w-1]
// but actually k can not be w-1, because 2<<(w-1) = INT_MIN != 2^(w-1)
// so divide_power2(INT_MIN, 31) = -1, but INT_MIN / (1 << 31) = 1
int divide_power2(int x, int k)
{
    // tow's complement division is roundint to zero
    // if x >= 0, round donw, x/2^k = (x + 0) >> k
    // if x < 0, round up, x/2^k = (x + bias) >> k
    int bias = (1 << k) - 1;
    // if x >= 0, x >> 31 = 0, 0 & bias = 0
    // if x < 0, x >> 31 = -1, -1 & bias = bias
    bias &= (x >> 31);
    return (x + bias) >> k; 
}

void test_divide_power2_impl(int x)
{
    int w = sizeof(int) << 3;
    for (int k = 0; k < w; ++k)
    {
        if (divide_power2(x, k) != x / (1 << k))
        {
            printf("divide_power2(%d, %d) = %d, %d / (1 << %d) = %d\n",
                x, k, divide_power2(x, k), x, k, x / (1 << k));
            return;
        }
    }
}

void test_divide_power2()
{
    test_divide_power2_impl(0x12345678);
    test_divide_power2_impl(0x87654321);
    test_divide_power2_impl(0);
    test_divide_power2_impl(-1);
    test_divide_power2_impl(1);
    test_divide_power2_impl(INT_MAX);
    test_divide_power2_impl(INT_MIN);
}

int main()
{
    test_divide_power2();
}
