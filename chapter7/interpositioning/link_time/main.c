// 2023/3/3
// zhangzhong

#include <malloc.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int *p = malloc(32);
  free(p);
  return 0;
}
