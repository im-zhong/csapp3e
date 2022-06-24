// 2022/6/24
// zhangzhong
// Homework Problem 2.69

#include <assert.h>

// Do rotating left shift.
// n: [0, w-1]
unsigned rotate_left(unsigned x, int n)
{
	int w = sizeof(unsigned) << 3;
	int rotate = 0;
	(n != 0) && (rotate = x >> (w - n));
	return (x << n) | rotate;
}

void test_rotate_left()
{
	assert(rotate_left(0x12345678, 0) == 0x12345678);
	assert(rotate_left(0x12345678, 4) == 0x23456781);
	assert(rotate_left(0x12345678, 20) == 0x67812345);
}

int main(int argc, char* argv[])
{
	test_rotate_left();
}