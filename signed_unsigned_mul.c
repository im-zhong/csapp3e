// 2021/10/19
// signed mul and unsigned mul result's low bit is the same

#include <stdio.h>
#include <assert.h>

int main()
{
    int x = 1;
    int y = -1;
    unsigned ux = x;
    unsigned uy = y;
    assert(x * y == ux * uy);
}
