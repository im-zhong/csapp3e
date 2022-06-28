// 2021/10/24
// zhangzhong
// Homework Problem 2.97

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned float_bits;

float u2f(unsigned u) { return *(float *)&u; }

unsigned f2u(float f) { return *(unsigned *)&f; }

void show_bytes(unsigned u) {
  for (unsigned i = 0; i < sizeof(u) * 8; ++i, u <<= 1) {
    if (u & INT_MIN) {
      printf("%c", '1');
    } else {
      printf("%c", '0');
    }

    if (i == 0 || i == 8) {
      printf(",");
    }
  }
  printf("\n");
}

void show_float_bytes(float_bits f) {
  printf("f = %f, bytes = ", u2f(f));
  show_bytes(f);
}

unsigned find_high_one_bit(unsigned u) {
  for (unsigned k = sizeof(u) * 8 - 1; k < UINT_MAX; --k, u <<= 1) {
    if (u & INT_MIN) {
      return k;
    }
  }
}

float_bits float_i2f(int i) {
  unsigned sign = 0;
  unsigned exp = 0;
  unsigned frac = 0;
  // find the first 1, except the i
  if (i < 0) {
    sign = 1;
    // 先处理特殊值， Tmin
    // 其他的值可以直接转成整数处理
    if (i == INT_MIN) {
      frac = 0;
      exp = 31 + 127;

      return (sign << 31) | (exp << 23) | frac;
    } else {
      // 转成正数处理
      i = ~i + 1;
    }
  }

  // i >= 0
  if (i == 0) {
    return i;
  }

  // i > 0
  // find the first 1
  unsigned k = find_high_one_bit(i);
  unsigned bias = 127;
  unsigned frac_size = 23;
  if (k <= 23) {
    // case 1, no need round

    i = i << (23 - k);
    frac = i & 0x7fffff;
    exp = k + bias;

  } else {
    // k > 23, k < 32

    bool round_flag = false;
    unsigned round_bits = k - 23;
    unsigned round_mask = (1 << round_bits) - 1;
    unsigned round = i & round_mask;
    unsigned round_even = 1 << (round_bits - 1);

    i = i >> round_bits;
    frac = i & 0x7fffff;
    exp = k + bias;

    if (round < round_even) {
      // nothing to do
    } else if (round > round_even) {
      // + 1
      // 如果frac == 1111111...111
      // 此时

      round_flag = true;
    } else {
      // round even
      // if frac's lowest bit is 1, add 1
      if (frac & 0x1 == 1) {
        round_flag = true;
      }
    }

    if (round_flag) {
      if (frac == 0x7fffff) {
        frac = 0;
        exp++;
      } else {
        frac++;
      }
    }
  }

  return (sign << 31) | (exp << 23) | frac;
}

int main() {
  unsigned u = 0;
  for (; u < UINT_MAX; ++u) {
    if (float_i2f(u) != f2u(((float)((int)u)))) {
      printf("i = %d\n", (int)u);
      show_float_bytes(f2u(((float)((int)u))));
      show_float_bytes(float_i2f(u));
      exit(EXIT_FAILURE);
    }
  }
  assert(float_i2f(u) == f2u(((float)((int)u))));

  int i = 1;
  float_i2f(1);
}