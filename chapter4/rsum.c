// 2022/10/31
// zhangzhong
// Practice Problem 4.4

long rsum(long *start, long count) {
  if (count <= 0) {
    return 0;
  }
  return *start + rsum(start + 1, count - 1);
}
