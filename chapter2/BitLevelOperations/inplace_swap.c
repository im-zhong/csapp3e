// 2022/6/20
// zhangzhong
// Practice Problem 2.10

#include <stdio.h>

void swap(int* x, int* y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

void inplace_swap(int* x, int* y)
{
	// 1. ^ is commutative and associative
	// 2. a ^ a = 0; 0 ^ a = a

	// initially, *x = a, *y = b
	*y = *x ^ *y;	// *x = a, *y = a ^ b
	*x = *x ^ *y;	// *x = a ^ a ^ b = b， *y = a ^ b
	*y = *x ^ *y;	// *x = b, *y = b ^ a ^ b = a
}

void test_inplace_swap()
{
	int x = 1;
	int y = 2;
	printf("before swap: x = %d, y = %d\n", x, y);
	inplace_swap(&x, &y);
	printf("after swap: x = %d, y = %d\n", x, y);
}


int main(int argc, char* argv[])
{
	test_inplace_swap();
}