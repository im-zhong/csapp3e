// 2022/10/31
// zhangzhong
// Practice Problem 4.5

long abssum(long *start, long count) {
  long sum = 0;
  while (count) {
    if (*start < 0) {
      sum += -(*start);
    } else {
      sum += *start;
    }
    start++;
    count--;
  }
  return sum;
}
