// 2022/7/6
// zhangzhong
// Figure 3.40

#include <stdio.h>

// -fno-stack-protector
// implementation of library function gets()
char *gets(char *s) {
  int c;
  char *dest = s;
  while ((c = getchar()) != '\n' && c != EOF)
    *dest++ = c;
  if (c == EOF && dest == s) {
    // no characters read
    return NULL;
  }
  // terminate string
  *dest++ = '\0';
  return s;
}

void echo() {
  char buf[8];
  gets(buf);
  puts(buf);
}

int main(int argc, char *argv[]) { echo(); }