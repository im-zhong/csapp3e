// 2021/10/19
// test for negative unsigned

#include <stdio.h>

int main() {
  unsigned ux = 1;
  unsigned uy = -ux;
  unsigned uz = -1;

  printf("%u\n", uy);
  printf("%u\n", uz);
}
