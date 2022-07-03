// 2021/10/31
// zhangzhong
// Figure 3.19 Code for do-while version of factorial program

// why use add 1 not inc
// https://stackoverflow.com/questions/36510095/inc-instruction-vs-add-1-does-it-matter
// https://stackoverflow.com/questions/13383407/is-add-1-really-faster-than-inc-x86

long fact_do(long n) {
  long result = 1;
  do {
    result *= n;
    // n = n - 1;
    --n;
  } while (n > 1);
  return result;
}

long fact_do_goto(long n) {
  long result = 1;
loop:
  result *= n;
  n = n - 1;
  if (n > 1)
    goto loop;
  return result;
}
