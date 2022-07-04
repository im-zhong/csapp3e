// 2022/7/4
// zhangzhong
// Practice Problem 3.35

long rfun(unsigned long x) {
  if (x == 0)
    return 0;
  unsigned long nx = x >> 2;
  long rv = rfun(nx);
  return x + rv;
}
