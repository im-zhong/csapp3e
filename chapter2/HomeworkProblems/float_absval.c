// 2021/10/21
// zhangzhong

#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

typedef unsigned float_bits;

float u2f(unsigned u)
{
    return *(float*)&u;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

// compute |f|. if f is nan, return f
float_bits float_absval(float_bits f)
{
    // unsigned sign = f >> 31;
    // unsigned exp = f >> 23 & 0xff;
    // unsigned frac = f & 0x7fffff;

    // // if f is nan, return f
    // // 这里跟上一题是一样的
    // // 我们可以将这个函数简化，实际上只需要将符号位置零即可 return f & 0x7fffffff;
    // // if (exp == 0xff && frac != 0)
    // // {
    // //     return f;
    // // }

    // // make sign be 0
    // sign = 0;
    // return (sign << 31) | (exp << 23) | frac;
    return f & 0x7fffffff;
}

int main(int argc, char *argv[])
{
    unsigned u = 0;
    for (; u < UINT_MAX; ++u)
    {
        if (float_absval(u) != f2u(fabsf(u2f(u))))
        {
            printf("%u\n", u);
            printf("%f\n", u2f(u));
        }
    }
    assert(float_absval(u) == f2u(fabsf(u2f(u))));
}