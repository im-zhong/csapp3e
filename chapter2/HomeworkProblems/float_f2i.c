// 2021/10/24
// zhangzhong
// 

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

int float_f2i(float_bits f)
{
    unsigned sign = f >> 31;
    unsigned exp = (f >> 23) & 0xff;
    unsigned frac = f & 0x7fffff;
    // printf("frac = ");
    // show_bytes(frac);

    // f < 1.0f, i should be zero
    unsigned bias = 127;
    if (exp < 127u)
    {
        return 0;
    }
    else
    {
        // exp = 127, frac = 0; then f = 1.0f
        // normalized, M = 1.frac
        frac = 0x800000 | frac;
    // printf("frac = ");
    // show_bytes(frac);
        // exp >= 127
        unsigned E = exp - bias;
        if (E <= 23)
        {
            int k = 23 - E;
            frac = frac >> k;
        }
        else if (E < (23 + 8))
        {
            int k = E - 23;
            frac = frac << k;
        }
        else
        {
            return 0x80000000;
        }

        // frac
        int i = frac;
        if (sign == 1)
        {
            // -i
            i = ~i + 1;
        }
        return i;
    }

}

int main()
{
    unsigned u = 0;
    for (; u < UINT_MAX; ++u)
    {
        if (float_f2i(u) != (int)u2f(u))
        {
            // printf("i = %d\n", (int)u);   
            show_float_bytes(u);          
            show_bytes((int)u2f(u));                      
            // printf("u2f(u) = %f\n", u2f(u));   
            show_bytes(float_f2i(u));                          
            // printf("float_f2i(u) = %d\n", float_f2i(u));           

            // show_float_bytes(float_f2i(u));
            // printf("(int)u2f(u) = %d\n", (int)u2f(u));                           
            // printf("f2u((int)u2f(u)) = %u\n", f2u((int)u2f(u)));
            
            exit(EXIT_FAILURE);
        }
    }
    assert(float_f2i(u) == (int)u2f(u));
}