// 2021/10/31
// zhangzhong
// Practice Problem 3.27

long fact_for(long n) {
  long i;
  long result = 1;
  for (i = 2; i <= n; i++)
    result *= i;
  return result;
}

long fact_for_jm_goto(long n) {
  long i = 2;
  long result = 1;
  goto test;
loop:
  result *= i;
  i++;
test:
  if (i <= n)
    goto loop;
  return result;
}

long fact_for_gd_goto(long n) {
  long i = 2;
  long result = 1;
  if (i > n) // n <= 1
    goto done;
loop:
  result *= i;
  i++;
  if (i <= n)
    goto loop;
done:
  return result;
}
