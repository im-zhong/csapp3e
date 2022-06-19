// 2022/6/19
// zhangzhong
// Conversion among bin, oct, dev, hex.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// create a character table, 

// 可以支持任意的数字，但是不能超过机器自身的数字
int word_size = 64;


bool is_bin(const char* str, size_t size)
{

}

bool is_oct(const char* str, size_t size)
{

}

bool is_dec(const char* str, size_t size)
{

}

bool is_hex(const char* str, size_t size)
{

}

// 这个不需要保存呀，一次位运算而已，直接计算即可
long bit_power[64] = {
	1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8
};

// how to i get a string ?? like a scanf, i need a pointer to a pointer
void bin2dec(const char* bin, size_t size)
{
	// actually i should restrict the bits of the bin. so if some one input some large word i could check it
	// and how do i convert bin to dec??
	// i alwayds use bit power

	long dec = 0;
	for (size_t i = 0; i < size; ++i)
	{
		// bin[i] is a string like "01001011"
		// the first, i need check if its "0" or "1"
		// then, i should convert "0" to 0, "1" to 1
		// then add all these to the dec
		if (bin[i] == '0' || bin[i] == '1')
		{
			if ((bin[i] - '0') == 1)
			{
				dec += 1 << (63-i);
			}
		}
	}
}

// 我们还可以有补码, 最长是64bit
// tcb
void tcb2dec(const char* tcb, size_t size)
{
	// 补码的最高位是符号位 -1 * bit_power
	// 其余的和最高位一样
	// 不过，如果我限制了位数，那么我想要研究小的数字就显得非常不方便
	// 我可以通过参数指定目前机器的位数，这样就更加合理
}

// 如果是16进制 , 64 / 4 = 16bit
// 如果是八进制，最长是 64/3 = 21.3 也就是21位
// 八进制用的太少了，不支持了


int main(int argc, char* argv[])
{

}