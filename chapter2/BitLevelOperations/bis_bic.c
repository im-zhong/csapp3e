// 2022/6/20
// zhangzhong
// Practice Problem 2.13

#include <assert.h>

// bis: bit set, setting result to 1 at each bit position where mask is 1
int bis(int x, int m)
{
	return x | m;
}

// bic: bit clear, setting result to 0 at each bit position where mask is 1
int bic(int x, int m)
{
	return x & ~m;
}

int bool_or(int x, int y)
{
	return bis(x, y);
}

// x ^ y = (x & ~y) | (y & ~x)
int bool_xor(int x, int y)
{
	return bis(bic(x, y), bic(y, x));
}

void test_bool_or()
{

}

void test_bool_xor()
{
	assert((123 ^ 456) == bool_xor(123, 456));
}

int main(int argc, char* argv[])
{
	test_bool_xor();
}
