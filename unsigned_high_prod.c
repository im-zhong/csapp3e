// 2021/10/14
// zhangzhong
// 2.75

#include <assert.h>
#include <stdio.h>

int signed_high_prod(int x, int y)
{
    // sizeof(long) is 8
    long lx = x;
    long ly = y;
    long lz = lx * ly;
    lz >>= 32;
    return lz;
}

int sign(int x)
{
    return x < 0 ? 1 : 0;
}

unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    return signed_high_prod(x, y) + sign(x) * (int)y + sign(y) * (int)x;
}

unsigned unsigned_high_prod_correct(unsigned x, unsigned y)
{
    unsigned long lux = x;
    unsigned long luy = y;
    unsigned long luz = lux * luy;
    return luz >> 32;
}

int main()
{
    assert(unsigned_high_prod(1, 2) == unsigned_high_prod_correct(1, 2));
    assert(unsigned_high_prod(1, -2) == unsigned_high_prod_correct(1, -2));
}
