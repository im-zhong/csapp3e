// 2021/10/21
// zhangzhong
// Homework Problem 2.92

#include <assert.h>
#include <limits.h>
#include <stdio.h>

typedef unsigned float_bits;

float u2f(unsigned u) { return *(float *)&u; }

unsigned f2u(float f) { return *(unsigned *)&f; }

float_bits float_negate(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xff;
  unsigned frac = f & 0x7fffff;

  // if f is Nan, return f
  if (exp == 0xff && frac != 0) {
    return f;
  }

  // revert sign
  sign == 0 ? (sign = 1) : (sign = 0);
  return (sign << 31) | (exp << 23) | frac;
}

float_bits float_negate_v2(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xff;
  unsigned frac = f & 0x7fffff;

  // if f is Nan, return f
  // if (exp == 0xff && frac != 0)
  // {
  //     return f;
  // }
  // otherwise f is nan or not, just flip the sign bit

  // revert sign
  sign == 0 ? (sign = 1) : (sign = 0);
  return (sign << 31) | (exp << 23) | frac;
}

int main(int argc, char *argv[]) {
  // unsigned u = 0;
  // for (; u < UINT_MAX; ++u)
  // {
  //     if (float_negate(u) != f2u(-u2f(u)))
  //     {
  //         printf("%u\n", u);
  //         printf("%f\n", u2f(u));
  //     }
  // }
  // assert(float_negate(u) == f2u(-u2f(u)));

  // nan
  // unsigned u = 2139327102;                                        //
  // 01111111100000111000101001111110(2) printf("u = %u\n", u); // 2139327102
  // printf("u2f(u) = %f\n", u2f(u));                                // nan
  // printf("float_negate(u) = %u\n", float_negate(u));              //
  // 2139327102 printf("u2f(float_negate(u)) = %f\n", u2f(float_negate(u))); //
  // nan printf("-u2f(u) = %f\n", -u2f(u));                              // -nan
  // printf("f2u(-u2f(u)) = %u\n", f2u(-u2f(u)));                    //
  // 4286810750 = 11111111100000111000101001111110(2)
  // 可以很明显的看出来，在这台机器上，实际上不会区分一个浮点是是不是nan,
  // 他仅仅反转了符号位而已 这样对比一下第一个和最后一个输出更加明显
  // 01111111100000111000101001111110
  // 11111111100000111000101001111110

  unsigned u = 0;
  for (; u < UINT_MAX; ++u) {
    if (float_negate_v2(u) != f2u(-u2f(u))) {
      printf("%u\n", u);
      printf("%f\n", u2f(u));
    }
  }
  assert(float_negate_v2(u) == f2u(-u2f(u)));
}
