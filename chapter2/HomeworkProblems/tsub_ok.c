// 2022/6/214
// zhangzhong
// Homework Problem 2.74

#include <assert.h>
#include <limits.h>
#include <stdio.h>

int tadd_ok(int x, int y) {
  return !((x > 0 && y > 0 && x + y < 0) || (x < 0 && y < 0 && x + y >= 0));
}

// Determine whether arguments can be subtracted without overflow
int tsub_ok(int x, int y) {
  if (y == INT_MIN) {
    return x < 0;
  } else {
    return tadd_ok(x, -y);
  }
}

int main(int argc, char *argv[]) {}
