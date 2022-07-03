// 2021/10/31
// zhangzhong
// Practice Problem 3.23

long dw_loop(long x) {
  long y = x * x;
  long *p = &x;
  long n = 2 * x;
  do {
    x += y;
    (*p)++;
    n--;
  } while (n > 0);
  return x;
}
