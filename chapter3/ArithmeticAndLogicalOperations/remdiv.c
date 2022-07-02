// 2022/7/2
// zhangzhong

// 商: quotient, 余数: remainder
void remdiv(long x, long y, long *qp, long *rp) {
  long q = x / y;
  long r = x % y;
  *qp = q;
  *rp = r;
}
