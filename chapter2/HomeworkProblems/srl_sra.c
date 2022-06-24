// 2022/6/24
// zhangzhong
// Homework Problem 2.63

#include <assert.h>
#include <limits.h>
#include <stdio.h>

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

// k = [0, w], low n bit is 1, others is 0
int get_mask(int k)
{
	int mask = 0;
	int w = sizeof(int) << 3;
	(k == w) && (mask = -1);
	(k != w) && (mask = (1 << k) - 1);
	return mask;
}

// shift right logicallly, (unsigned)x >> k
unsigned srl(unsigned x, int k)
{
	// perform shift arithmetically
	unsigned xsra = (int)x >> k;
	// just clear the high k bits of xsra
	int w = sizeof(int) << 3;
	// mask = k*0,(w-k)*1
	// if k is zero, then w - k == w, but shift can only within [0, w-1]
	int mask = get_mask(w - k);
	return xsra & mask;
}

// shfit right arithmetically, (int)x >> k
int sra(int x, int k)
{
	// perform shift logically
	int xsrl = (unsigned)x >> k;
	// get sign bit
	// if x >= 0, sign = 000...0 = 0;
	// if x < 0, sign = 100...0 = INT_MIN;
	int sign = x & INT_MIN;

	int w = sizeof(int) << 3;
	// mask = k*0,(w-k)*1
	int mask = get_mask(w - k);
	// use logical and's  properties
	// Ture && expr will run; False && expr will not run
	// if x >= 0, sign is false, xsra = xsrl
	// if x < 0, sign is true, xsra = bit set in high k bit of xsrl
	sign && (xsrl |= ~mask);
	return xsrl;
}

void test_srl()
{
	int w = sizeof(int) << 3;

	unsigned x = 0x12345678;
	for (int k = 0; k < w; ++k)
	{
		assert(srl(x, k) == x >> k);
	}

	x = 0x87654321;
	for (int k = 0; k < w; ++k)
	{
		assert(srl(x, k) == x >> k);
	}
}

void test_sra()
{
	int w = sizeof(int) << 3;

	int x = 0x12345678;
	for (int k = 0; k < w; ++k)
	{
		assert(sra(x, k) == x >> k);
	}

	x = 0x87654321;
	for (int k = 0; k < w; ++k)
	{
		assert(sra(x, k) == x >> k);
	}
}

int main(int argc, char* argv[])
{
	test_srl();
	test_sra();
}