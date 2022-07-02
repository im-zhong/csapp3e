// 2022/7/2
// zhangzhong
// Practice Problem 3.14

// A
// movq	%rdi, %rax
// notq	%rax
// shrq	$63, %rax
int testA(long a) { return a >= 0; }

// B
int testB1(short a) { return a == 0; }

int testB2(unsigned short a) { return a == 0; }

// C
// testb	%dil, %dil
// setne	%al
// movzbl	%al, %eax
int testC(unsigned char a) { return a > 0; }

// D
int testD(int a) { return a <= 0; }
