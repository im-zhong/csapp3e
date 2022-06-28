// 2022/6/24
// zhangzhong
// Homework Problem 2.70

#include <assert.h>
#include <limits.h>
#include <stdio.h>

// return 1 when x can be represented as an n-bit, 2's complement number, 0
// oterwise n: [1, w]
int fits_bits(int x, int n) {
  // only when high (w-n+1) bits is all 0 or all 1, will x fits in n bits

  // mask to get high (w-n+1) bits, clear low (n-1) bits
  int mask = ~0u << (n - 1);
  // get high (w-n+1) bits and arithmetic right shift to fulfill w bits
  // so the type of mask must be int
  x = (x & mask) >> (n - 1);
  // only when x is all 0 or all 1, will fits in n bits
  return !x || !~x;
}

// just check the range, which is the simplest way
int fits_bits_normal(int x, int n) {
  // if n == w, this expr will overflow to INT_MAX
  // but gcc's optimizer seems not work correctly
  // int max = (1 << (n - 1)) - 1;
  int max = 0;
  if (n == (sizeof(int) << 3)) {
    max = INT_MAX;
  } else {
    max = (1 << (n - 1)) - 1;
  }
  int min = -max - 1;
  // printf("max = %d, min = %d\n", max, min);
  // printf("%d, %d\n", x >= min, x <= max);
  return x >= min && x <= max;
}

void test_fits_bits() {
  for (int i = 1; i <= (sizeof(int) << 3); ++i) {
    for (int x = INT_MIN; x < INT_MAX; ++x) {
      // printf("%d\n", x);
      if (fits_bits(x, i) != fits_bits_normal(x, i)) {
        printf("fits_bits(%d, %d) = %d, fits_bits_normal(%d, %d) = %d", x, i,
               fits_bits(x, i), x, i, fits_bits_normal(x, i));
        return;
      }
    }
    assert(fits_bits(INT_MAX, i) == fits_bits_normal(INT_MAX, i));
    printf("%d\n", i);
  }
}

int main(int argc, char *argb[]) { test_fits_bits(); }