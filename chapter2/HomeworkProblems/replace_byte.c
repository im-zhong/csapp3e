// 2022/6/24
// zhangzhong
// Homework Problems 2.60

#include <assert.h>
#include <stdio.h>

unsigned replace_byte(unsigned x, int i, unsigned char b) {
  if (i < 0 || i >= sizeof(unsigned)) {
    return x;
  }

  // mask = 0x00ff0000
  unsigned mask = 0xff << (i << 3);
  // x = 0x12003456
  x = x & ~mask;
  // y = 0x000000ab
  unsigned y = b;
  // y = 0x00ab0000
  y = y << (i << 3);
  return x | y;
}

void test_replace_byte() {
  assert(replace_byte(0x12345678, 2, 0xab) == 0x12ab5678);
  assert(replace_byte(0x12345678, 0, 0xab) == 0x123456ab);
}

int main(int argc, char *argv[]) { test_replace_byte(); }