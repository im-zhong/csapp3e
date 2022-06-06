// 2021/10/24
// zhangzhong

#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

typedef unsigned float_bits;

float u2f(unsigned u)
{
    return *(float *)&u;
}

unsigned f2u(float f)
{
    return *(unsigned *)&f;
}

void show_bytes(unsigned u)
{
    for (unsigned i = 0; i < sizeof(u) * 8; ++i, u <<= 1)
    {
        if (u & INT_MIN)
        {
            printf("%c", '1');
        }
        else
        {
            printf("%c", '0');
        }

        if (i == 0 || i == 8)
        {
            printf(",");
        }
    }
    printf("\n");
}

void show_float_bytes(float_bits f)
{
    printf("f = %f, bytes = ", u2f(f));
    show_bytes(f);
}

float_bits float_half(float_bits f)
{
    unsigned sign = f >> 31;
    unsigned exp = (f >> 23) & 0xff;
    unsigned frac = f & 0x7fffff;
    
    if (exp == 0 || exp == 1)
    {
        // case 1. f is nonnormalized or exp is 000..1
        // 有些特殊的情况，如果frac的末位是1，那么在右移一位之后，我们需要向偶数舍入
        if ((frac & 0x1) == 1)
        {
            unsigned exp_frac = (f & INT_MAX) >> 1;
            // round to even
            if ((exp_frac & 1) == 1)
            {
                exp_frac += 1;
            }
            return (sign << 31) | exp_frac;
        }
        else
        {
            return (sign << 31) | ((f & INT_MAX) >> 1);
        }
        
    }
    else if (exp == 0xff)
    {
        // infinite or nan
        // weried rule
        if (frac == 0)
        {
            // infinite, just return f
            return f;
        }
        else
        {
            // nan, frac | 0x400000
            return f | 0x400000;
        }
        
    }
    else
    {
        // case 2. normalized, except exp is 1
        exp--;
        return (sign << 31) | (exp << 23) | frac;
    }
}

int main()
{
    // unsigned u = 0;

    // unsigned sign = 0;
    // unsigned exp = 0;
    // unsigned frac = ~0 & 0x7fffff;

    // float_bits f = (sign << 31) | (exp << 23) | frac;
    // show_float_bytes(f);
    // show_float_bytes(0.5f * f);

    unsigned u = 0;
    for (; u < UINT_MAX; ++u)
    {
        if (float_half(u) != f2u(0.5f * u2f(u)))
        {
            printf("u = %u\n", u);   
            show_float_bytes(u);                                    
            printf("u2f(u) = %f\n", u2f(u));                             
            printf("float_half(u) = %u\n", float_half(u));           
            printf("u2f(float_half(u)) = %f\n", u2f(float_half(u)));
            show_float_bytes(float_half(u));
            printf("0.5f * u2f(u) = %f\n", 0.5f * u2f(u));                           
            printf("f2u(0.5f * u2f(u)) = %u\n", f2u(0.5f * u2f(u)));
            show_float_bytes(f2u(0.5f * u2f(u)));
            exit(EXIT_FAILURE);
        }
    }
    assert(float_half(u) == f2u(0.5f * u2f(u)));
}