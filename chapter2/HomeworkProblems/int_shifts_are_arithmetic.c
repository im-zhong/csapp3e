// 2022/6/24
// zhangzhong
// Homework Problems 2.62

#include <stdio.h>

int int_shifts_are_arithmetic()
{
	return -1 >> 1 == -1;
}

int main(int argc, char* argv[])
{
	if (int_shifts_are_arithmetic())
	{
		printf("int right shifts are arithmetic\n");
	}
	else
	{
		printf("int right shifts are logical\n");
	}
}