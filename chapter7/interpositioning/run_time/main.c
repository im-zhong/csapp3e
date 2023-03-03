// 2023/3/3
// zhangzhong

// #include <malloc.h>
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int *p = malloc(32);
  free(p);
  return 0;
}
