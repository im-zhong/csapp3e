// 2022/7/4
// zhangzhong
// Figure 3.35 Code for recursive factorial program

long rfact(long n) {
  long result = 1;
  if (n <= 1)
    result = 1;
  else
    result = n * rfact(n - 1);
  return result;
}
