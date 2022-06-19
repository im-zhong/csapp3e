// 2022/6/19
// zhangzhong
// figure 2.4 Code to print the byte representation of program objects.

#include <stdio.h>
#include <string.h>

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

void test_show_bytes()
{
	int i = 0x12345678;
	show_bytes((byte*)&i, sizeof(i));
	float f = (float)i;
	show_bytes((byte*)&f, sizeof(f));
	int* pi = &i;
	show_bytes((byte*)&pi, sizeof(pi));
	// practice problem 2.7
	const char* s = "abcdef";
	show_bytes((byte*)s, strlen(s));
}

int main(int argc, char* argv[])
{
	test_show_bytes();
}
