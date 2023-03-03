// 2023/3/3
// zhangzhong

#include <stddef.h>

#define malloc(size) mymalloc(size)
#define free(ptr) myfree(ptr)

void *mymalloc(size_t size);
void myfree(void *ptr);
