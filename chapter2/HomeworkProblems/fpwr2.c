// 2021/10/21
// zhangzhong
// Homework Problem 2.90

#include <assert.h>
#include <float.h>
#include <stdio.h>

float u2f(unsigned u) { return *(float *)&u; }

float fpwr2(int x) {
  unsigned exp = 0u;
  unsigned frac = 0u;
  unsigned u = 0u;

  // the smallest positive number
  int bias = 127; // 2^7 - 1
  int float_frac = 23;
  // nonnormalized E is 1 - bias = -126
  // and the smallest frac is 2^(-23)
  // so the min power is -(23 + 126) = -149
  if (x < -149) {
    // too small, return 0
    exp = 0u;
    frac = 0u;
  }
  // the smallest normalized number is 1.0 * 2^(1 - bias)
  // so the min power is 1 - 127 = -126
  else if (x < -126) {
    // [-149, -127] is nonnormalized number
    exp = 0u;
    frac = 1 << (float_frac - (1 - bias - x));
  }
  // the biggest normallized number is 1.0 * 2^bias
  // so the max poser is bias
  else if (x < 127 + 1) {
    // [-126, 127]
    // normalized number
    exp = x + bias;
    frac = 0;
  } else {
    // too big, just positive infinite
    // exp is 11111111, becase float has 8-bit exp
    exp = 0xff;
    frac = 0u;
  }
  u = exp << 23 | frac;
  return u2f(u);
}

int main(int argc, char *argv[]) {
  assert(fpwr2(0) == 1.0f);
  assert(fpwr2(-1) == 0.5f);
  assert(fpwr2(1) == 2.0f);

  assert(fpwr2(-10086) == 0.0f);
  assert(fpwr2(10) == 1024.0f);
  assert(fpwr2(10086) == (FLT_MAX * FLT_MAX));
}
