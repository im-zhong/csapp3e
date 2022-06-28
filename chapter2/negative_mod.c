// 2021/10/19
// zhangzhong
// negative div and mod

#include <stdio.h>

int main() {
  int x = 3;
  int y = 2;
  printf("%d, %d\n", x / y, x % y);

  x = -3;
  y = 2;
  printf("%d, %d\n", x / y, x % y);

  x = 3;
  y = -2;
  printf("%d, %d\n", x / y, x % y);

  x = -3;
  y = -2;
  printf("%d, %d\n", x / y, x % y);
}
