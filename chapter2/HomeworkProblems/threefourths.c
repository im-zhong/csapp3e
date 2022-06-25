// 2021/10/14
// zhangzhong
// 2.80
//

#include <assert.h>
#include <limits.h>

int threefourths(int x) {
  // 首先保存最低两位，这样除以4就不会损失精度
  int b1b0 = x & 0x3;
  // 然后计算 x / 4 * 3, 这样一定不会溢出
  x = x & ~0x3;
  (INT_MIN & x) && (x += (3));
  x >>= 2;
  x += (x << 1);

  // then calculate b1b0 * 3 / 4
  b1b0 += (b1b0 << 1);
  // b1b0 must be positive
  b1b0 >>= 2;

  return x + b1b0;
}

int threefourthsv2(int x) {
  int b1b0 = x & 0x3;
  x = (x >> 1) + (x >> 2);
  b1b0 == 3 && (x += 1);
  (b1b0 != 0) && (INT_MIN & x) && (x += 1);
  return x;
}

int threefourthsv3(int x) {
  int b1b0 = x & 0x3;
  x = (x >> 1) + (x >> 2);
  // b1b0 == 3 && (x += 1);
  // (b1b0 != 0) && (INT_MIN & x) && (x += 1);
  if (b1b0 != 0) {
    if (x >= 0) {
      if (b1b0 == 3) {
        x += 1;
      }

    } else {
      if (b1b0 == 3) {
        x += 2;
      } else {
        x += 1;
      }
    }
  }

  return x;
}

int threefourth_double(int x) {
  // int -> double will save precision
  double dx = x;
  dx = dx / 4.0 * 3.0;
  x = dx;
  return x;
}

void test_threefourth() {
  for (int x = 0; x < INT_MAX; ++x) {
    if (threefourthsv3(x) != threefourth_double(x)) {
      printf("threefourthsv3(%d) = %d, threefourth_double(%d) = %d\n", x,
             threefourthsv3(x), x, threefourth_double(x));
      return;
    }
  }
}

int main() {
  // assert(threefourthsv2(12345) == (12345 * 3 / 4));
  // assert(threefourthsv2(2000000000) == (2000000000 / 4 * 3));
  // assert(threefourthsv2(2000000002) == 1500000001);
  // assert(threefourthsv2(-2000000000) == (-2000000000 / 4 * 3));
  // assert(threefourthsv2(-2000000003) == -1500000002);
  // assert(threefourths(2000000000) == (2000000000 * 3 / 4));

  assert(threefourthsv2(127) == (95));
  assert(threefourthsv2(-127) == (-95));
  assert(threefourthsv2(-1) == (0));

  assert(threefourth_double(127) == (95));
  assert(threefourth_double(-127) == (-95));

  test_threefourth();
}
