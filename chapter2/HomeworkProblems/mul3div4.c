// 2021/10/14
// zhangzhong
// Homework Problem 2.79

#include <assert.h>
#include <limits.h>
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

// computes 3 * x / 4
int mul3div4(int x)
{
    // 3*x = x + 2*x = x + (x<<1)
    // x/4 = x / 2^2
   return divide_power2(x + (x << 1), 2);
}

void test_mul3div4()
{
    for (int x = INT_MIN; x < INT_MAX; ++x)
    {
        if (mul3div4(x) != 3*x/4)
        {
            printf("mul3div4(%d) = %d, 3*%d/4 = %d\n",
                x, mul3div4(x), x, 3*x/4);
        }
    }
    int x = INT_MAX;
    if (mul3div4(x) != 3*x/4)
    {
        printf("mul3div4(%d) = %d, 3*%d/4 = %d\n",
            x, mul3div4(x), x, 3*x/4);
    }
}

int main()
{
    test_mul3div4();
}
