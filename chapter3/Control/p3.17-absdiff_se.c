// 2021/10/28
// zhangzhong
// Practice Problem 3.17

long lt_cnt = 0;
long ge_cnt = 0;

long goto_diff_se(long x, long y) {
  long result;
  if (x < y)
    goto true_statement;
  // false statement
  ge_cnt++;
  result = x - y;
  goto done;
true_statement:
  lt_cnt++;
  result = y - x;
done:
  return result;
}
