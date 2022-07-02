// 2021/10/28
// zhangzhong
// Figure 3.16 Compilation of conditional statements

long lt_cnt = 0;
long ge_cnt = 0;

long absdiff_se(long x, long y) {
  long result = 0;
  if (x < y) {
    lt_cnt++;
    result = y - x;
  } else {
    ge_cnt++;
    result = x - y;
  }
  return result;
}
