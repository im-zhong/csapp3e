// 2022/7/4
// zhangzhong
// Figure 3.34 Code demostrating use of callee-saved registers

long Q(long x) { return x; }

long P(long x, long y) {
  long u = Q(y);
  long v = Q(x);
  return u + v;
}
