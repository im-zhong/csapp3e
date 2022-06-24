// 2022/6/24
// zhangzhong
// Homework Problems 2.65

#include <assert.h>
#include <stdio.h>
#include <limits.h>

typedef unsigned char byte;

void show_bytes(const byte* begin, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		// print as hex and width is 2
		printf("%.2x", begin[i]);
	}
	printf("\n");
}

// return 1 when x contains an odd numbers of 1s; 0 otherwise
// assume w = 32
// code should contain a total of at most 12 arithmetic, bitwise, and logical operations
int odd_ones(unsigned x)
{
	// (x ^ y) will contain the odd-even property of 1s!
	// for example, suppose x = 0101, y = 0100, the total 1s is odd.
	// x ^ y = 0101 ^ 0100 = 0001, althoung the total 1s number has changed, but it still is odd!

	x ^= (x >> 16);	// high 16 bits ^ low 16 bits, and save the odd-even property in the low 16 bits
	x ^= (x >> 8);	// then just look the low 16 bits, after this, the odd-even property is stored in low 8 bits
	x ^= (x >> 4);	// ...
	x ^= (x >> 2);	// ...
	x ^= (x >> 1);	// finally, if total 1s is odd, the last bits will be 1; otherwise will be 0
	return x & 1;
}

int odd_ones_normal(unsigned x)
{
	int ones_count = 0;
	while (x)
	{
		if ((x & 1) == 1)
		{
			++ones_count;
		}
		x >>= 1;
	}
	return (ones_count & 1) == 1;
}

// gcc -O3. test costs about few minites.
void test_odd_ones()
{
	for (unsigned x = 0; x < UINT_MAX; ++x)
	{
		if (odd_ones(x) != odd_ones_normal(x))
		{
			show_bytes((byte*)&x, sizeof(unsigned));
			printf("odd_ones(%u) = %d, odd_ones_normal(%u) = %d\n", x, odd_ones(x), x, odd_ones_normal(x));
			assert(odd_ones(x) == odd_ones_normal(x));
		}
		// printf("%u/%u\n", x, UINT_MAX);
	}
	assert(odd_ones(UINT_MAX) == odd_ones_normal(UINT_MAX));
}

int main(int argc, char* argv[])
{
	test_odd_ones();
}
