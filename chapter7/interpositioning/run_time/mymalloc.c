// 2023/3/3
// zhangzhong

#ifdef USE_MY_MALLOC
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// malloc wrapper function
void *malloc(size_t size) {
  void *(*mallocp)(size_t) = NULL;
  char *error = NULL;
  // static __thread bool printf_in_progress = false;

  mallocp = dlsym(RTLD_NEXT, "malloc");
  if ((error = dlerror()) != NULL) {
    fputs(error, stderr);
    exit(EXIT_FAILURE);
  }

  // call lic malloc
  void *ptr = mallocp(size);

  char buf[1024] = {'\0'};
  int len = sprintf(buf, "malloc(%zu) = %p\n", size, ptr);
  write(STDOUT_FILENO, buf, len);
  // if (!printf_in_progress) {
  //   printf_in_progress = true;
  //   fprintf(stdout, "malloc(%zu) = %p\n", size, ptr);
  //   printf_in_progress = false;
  // }
  return ptr;
}

// free wrapper function
void free(void *ptr) {
  void (*freep)(void *) = NULL;
  char *error = NULL;
  // static __thread bool printf_in_progress = false;

  if (!ptr) {
    return;
  }

  freep = dlsym(RTLD_NEXT, "free");
  if ((error = dlerror()) != NULL) {
    fputs(error, stderr);
    exit(EXIT_FAILURE);
  }

  // call libc free
  freep(ptr);
  char buf[1024] = {'\0'};
  int len = sprintf(buf, "free(%p)\n", ptr);
  write(STDOUT_FILENO, buf, len);
  // if (!printf_in_progress) {
  //   printf_in_progress = true;
  //   printf("free(%p)\n", ptr);
  //   printf_in_progress = false;
  // }
}
#endif
