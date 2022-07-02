// 2022/7/2
// zhangzhong
// Practice Problem 3.13

// A
int compA(int a, int b) { return a < b; }

// B
int compB(short a, short b) { return a >= b; }

// C
int compC(unsigned char a, unsigned char b) { return a <= b; }

// D
int compD1(long a, long b) { return a != b; }

int compD2(unsigned long a, unsigned long b) { return a != b; }

int compD3(int *a, int *b) { return a != b; }
