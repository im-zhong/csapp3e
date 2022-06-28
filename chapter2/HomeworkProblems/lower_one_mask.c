// 2022/6/24
// zhangzhong
// Homework Problem 2.68

#include <assert.h>
#include <stdio.h>

// mask with least significant n bits set to 1
int lower_one_mask(int n) {
  int w = sizeof(int) << 3;
  int mask = 0;
  (n == w) && (mask = ~0);
  (n != w) && (mask = (1 << n) - 1);
  return mask;
}

void test_lower_one_mask() {
  assert(lower_one_mask(6) == 0x3f);
  assert(lower_one_mask(17) == 0x1ffff);
  assert(lower_one_mask(0) == 0x0);
  assert(lower_one_mask(1) == 0x1);
  assert(lower_one_mask(32) == 0xffffffff);
}

int main(int argc, char *argv[]) { test_lower_one_mask(); }