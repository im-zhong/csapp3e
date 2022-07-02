// 2022/7/2
// zhangzhong
// Figure 3.11 C and assembly code for arithmetic function

long arith(long x, long y, long z) {
  long t1 = x ^ y;
  long t2 = z * 48;
  long t3 = t1 & 0x0f0f0f0f;
  long t4 = t2 - t3;
  return t4;
}
