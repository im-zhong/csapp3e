// 2022/6/24
// zhangzhong
// Homework Problem 2.64

#include <assert.h>

// return 1 when any odd bit of x equals 1; 0 otherwises
// assume w = 32
int any_odd_one(unsigned x) { return !!(0xaaaaaaaa & x); }

void test_any_odd_one() {
  assert(any_odd_one(0) == 0);
  assert(any_odd_one(1) == 0);
  assert(any_odd_one(2) == 1);
}

int main(int argc, char *argv[]) { test_any_odd_one(); }
