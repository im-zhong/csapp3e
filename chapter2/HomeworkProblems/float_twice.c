// 2021/10/21
// zhangzhong
// float_twice.c

#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

typedef unsigned float_bits;

float u2f(unsigned u)
{
    return *(float*)&u;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

float_bits float_twice(float_bits f)
{
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xff;
    unsigned frac = f & 0x7fffff;

    if (exp == 0)
    {
        // non normalized
        // left shift the whole number, except the sign
        // becase exp must be 00000000
        // so after the shift, the sign must be 0
        // so we use '|' to add the original sign bit
        // think a very interesting case:
        // when f = 0|00000000|1000...0 = 2^-1 * 2^-126
        // 2.0 * f will cause denormaolized to normolized, but how can we deal with it
        // we just right shift the whole frac into the exponent part, which is just right!
        // 2.0 * f = 2^-126 = 0|00000001|0000...0
        return (sign << 31) | (f << 1);
    }

    if (exp == 0xff)
    {
        // the f should be infinite or nan, whatever situation
        // we just return the f is correct

        if (frac == 0)
        {
            // infinite, just return f
            return f;
        }
        else
        {
            // this is strange, frac | 1000...., frac | 0x400000
            // but why ??
            // maybe relative
            // https://stackoverflow.com/questions/1565164/what-is-the-rationale-for-all-comparisons-returning-false-for-ieee754-nan-values?rq=1
            // https://stackoverflow.com/questions/10034149/why-is-nan-not-equal-to-nan?noredirect=1&lq=1
            frac = frac | 0x400000;
            return (sign << 31) | (exp << 23) | frac;
        }
    }

    // normalized
    exp += 1;
    // 这个操作可能溢出哦
    // 如果在计算完成之后，exp == 0xff, frac should be zero
    if (exp == 0xff)
    {
        frac = 0;
    }
    return (sign << 31) | (exp << 23) | frac;
}

int main(int argc, char *argv[])
{
    unsigned u = 0;
    for (; u < UINT_MAX; ++u)
    {
        if (float_twice(u) != f2u(2.0f * u2f(u)))
        {
            printf("u = %u\n", u);                                       
            printf("u2f(u) = %f\n", u2f(u));                             
            printf("float_twice(u) = %u\n", float_twice(u));           
            printf("u2f(float_twice(u)) = %f\n", u2f(float_twice(u))); 
            printf("2.0f * u2f(u) = %f\n", 2.0f * u2f(u));                           
            printf("f2u(2.0f * u2f(u)) = %u\n", f2u(2.0f * u2f(u)));
            // exit(EXIT_FAILURE);
            printf("---------\n");
        }
    }
    assert(float_twice(u) == f2u(2.0f * u2f(u)));   
}
