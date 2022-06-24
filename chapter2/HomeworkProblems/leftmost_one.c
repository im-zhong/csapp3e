// 2022/6/25
// zhangzhong
// Homework Problem 2.66

#include <assert.h>
#include <limits.h>
#include <stdio.h>

// generate mask indicating left most 1 in x
// assume w = 32
int leftmost_one(unsigned x)
{
	// suppose x = 0x1000...0000
	x |= x >> 1;	// after this, x = 1100...0000
	x |= x >> 2;	// after this, x = 11110000....0000
	x |= x >> 4;	// after this, x = 0xff000000
	x |= x >> 8;	// after this, x = 0xffff0000
	x |= x >> 16;	// after this, x = 0xffffffff
	// case 1. x = 0...01...1, then we just calculate (x >> 1) + 1
	// case 2. x = 1...1, (x >> 1) + 1 also works
	// case 3. x = 0...0, in this case we should not add the 1, so we add (x & 1) to fix it.
	return (x >> 1) + (x & 1);
}

int leftmost_one_normal(unsigned x)
{
	int k = -1;
	while (x)
	{
		x >>= 1;
		++k;
	}

	// k = [-1, 31]
	if (k == -1)
	{
		return 0;
	}
	return 1 << k;
}

void test_leftmost_one()
{
	// for (unsigned x = 0; x < UINT_MAX; ++x)
	// {
	// 	if (leftmost_one(x) != leftmost_one_normal(x))
	// 	{
	// 		printf("leftmost_one(%u) = %d, leftmost_one_normal(%u) = %d\n", x, leftmost_one(x), x, leftmost_one_normal(x));
	// 		return;
	// 	}
	// }
	assert(leftmost_one(UINT_MAX) == leftmost_one_normal(UINT_MAX));
}

int main(int argc, char* argv[])
{
	test_leftmost_one();
}