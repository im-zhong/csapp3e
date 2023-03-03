// 2023/3/3
// zhangzhong

#ifdef USE_MY_MALLOC
#include <malloc.h>
#include <stdio.h>

// malloc wrapper function
void *mymalloc(size_t size) {
  void *ptr = malloc(size);
  printf("malloc(%zu) = %p\n", size, ptr);
  return ptr;
}

// free wrapper function
void myfree(void *ptr) {
  free(ptr);
  printf("free(%p)\n", ptr);
}
#endif
