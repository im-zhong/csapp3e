// 2022/6/24
// zhangzhong
// Homework Problems 2.58

#include <stdio.h>

typedef unsigned char byte;

int is_little_endian()
{
	int i = 1;
	return *(byte*)&i == 1;
}

int main(int argc, char* argv[])
{
	if (is_little_endian())
	{
		printf("This machine is little endian.\n");
	}
	else 
	{
		printf("This machine is big endian.\n");
	}
}