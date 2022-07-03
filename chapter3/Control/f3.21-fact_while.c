// 2021/10/31
// zhangzhong
// Figure 3.21 C and assembly code for while version of factorial using
// guarded-do translation

// -O1
long fact_while(long n) {
  long result = 1;
  while (n > 1) {
    result *= n;
    n = n - 1;
  }
  return result;
}

// guarded-do
long fact_while_gd_goto(long n) {
  long result = 1;
  if (n <= 1)
    goto done;
loop:
  result *= n;
  n -= 1;
  if (n != 1)
    goto loop;
done:
  return result;
}
