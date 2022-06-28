// 2021/10/14
// zhangzhong
// Homework Problem 2.76

#include <assert.h>
#include <limits.h>
#include <memory.h>
#include <stdlib.h>

// if x * y is ok, return 1; otherwise overflow, return 0
int tmult_ok(int x, int y) {
  int p = x * y;
  return !x || p / x == y;
}

// allocates memory for an array of nmemb elements of size bytes each
void *my_calloc(size_t nmemb, size_t size) {
  // if any factor is 0, just return nullptr
  if (nmemb == 0 || size == 0) {
    return NULL;
  }

  // check if the multiplacation is overflow or not
  size_t mem_size = nmemb * size;
  if (mem_size / nmemb != size) {
    // if overflow, return nullptr
    return NULL;
  }

  // call malloc to allocate memory
  // if success, call memset to initiate to zero
  // otherwise, return nullptr
  void *mem_ptr = malloc(mem_size);
  if (!mem_ptr) {
    return NULL;
  }
  memset(mem_ptr, 0, mem_size);
  return mem_ptr;
}

void test_my_calloc() {
  void *p = my_calloc(1024, 64);
  assert(p);
  free(p);

  p = my_calloc(UINT_MAX, UINT_MAX);
  assert(!p);
}

int main(int argc, char *argv[]) { test_my_calloc(); }
