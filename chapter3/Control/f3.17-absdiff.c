// 2021/10/31
// zhangzhong
// Figure 3.17 Compilation of conditional statements using conditional
// assignment

// gcc -O1 -S use cmov, -Og does not work on my machine
// movq	%rsi, %rdx
// subq	%rdi, %rdx
// movq	%rdi, %rax
// subq	%rsi, %rax
// cmpq	%rsi, %rdi
// cmovl	%rdx, %rax
long absdiff(long x, long y) {
  long result;
  if (x < y)
    result = y - x;
  else
    result = x - y;
  return result;
}
