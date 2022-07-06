// 2022/7/6
// zhangzhong
// stack randomization

#include <stdio.h>

int main(int argc, char *argv[]) {
  long local;
  printf("local at %p\n", &local);
  return 0;
}
