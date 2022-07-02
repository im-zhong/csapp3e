// 2022/7/2
// zhangzhong
// Figure 3.7 C and assembly code for exchange routine

long exchange(long *xp, long y) {
  long x = *xp;
  *xp = y;
  return x;
}
