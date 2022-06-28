// 2021/10/20
// zhangzhong
// Homework Problem 2.84

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

unsigned f2u(float x) { return *(unsigned *)&x; }

bool float_le(float x, float y) {
  unsigned ux = f2u(x);
  unsigned uy = f2u(y);

  unsigned sx = ux >> 31;
  unsigned sy = uy >> 31;

  // return sx ? uy <= ux : ux <= uy;
  return (sx || sy) ? ((!ux && uy == -uy) ? ux <= uy : ux >= uy) : ux <= uy;
}

void test_float_le(float x, float y) {
  printf("--------\n");
  printf("x = %f, ux = %u\n", x, f2u(x));
  printf("y = %f, uy = %u\n", y, f2u(y));
  assert(float_le(x, y) == (bool)(x <= y));
  printf("--------\n");
}

int main() {
  printf("%u\n", f2u(0.0));
  printf("%u\n", f2u(-0.0));

  test_float_le(0.0, 1.1);
  test_float_le(1.1, 0);
  test_float_le(1.1, 1.1);
  test_float_le(-1.1, -2.2);
  test_float_le(-2.2, -1.1);
  test_float_le(-2.2, -2.2);

  test_float_le(0.0, -1.0);
  test_float_le(-1.0, 0.0);

  test_float_le(-1.0, -0.0);
  test_float_le(-0.0, -1.0);

  test_float_le(-0.0, 0.0);
  test_float_le(0.0, -0.0);
}
