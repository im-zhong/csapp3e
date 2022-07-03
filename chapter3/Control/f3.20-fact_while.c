// 2021/10/31
// zhangzhong
// Figure 3.20

long fact_while(long n) {
  long result = 1;
  while (n > 1) {
    result *= n;
    n = n - 1;
  }
  return result;
}

// jm: jump to middle
long fact_while_jm_goto(long n) {
  long result = 1;
  goto test; // jump to middle
loop:
  result *= n;
  n = n - 1;
test: // middle
  if (n > 1)
    goto loop;
  return result;
}
