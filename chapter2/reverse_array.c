// 2022/6/20
// zhangzhong
// Practice Problem 2.11

// first -> middle <- last
// for (;first < last; ++first, --last)

#include <stdio.h>

void inplace_swap(int *x, int *y) {
  // 1. ^ is commutative and associative
  // 2. a ^ a = 0; 0 ^ a = a

  // initially, *x = a, *y = b
  *y = *x ^ *y; // *x = a, *y = a ^ b
  *x = *x ^ *y; // *x = a ^ a ^ b = b， *y = a ^ b
  *y = *x ^ *y; // *x = b, *y = b ^ a ^ b = a
}

void reverse_array(int a[], size_t size) {
  // if size == 0? size - 1 = intmax, 直接缓冲区溢出
  if (size < 2) {
    return;
  }
  for (size_t first = 0, last = size - 1; first < last; ++first, --last) {
    inplace_swap(&a[first], &a[last]);
  }
}

void test_reverse_array() {}

int main(int argc, char *argv[]) { test_reverse_array(); }