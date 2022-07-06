// 2022/7/6
// zhangzhong
// Homework Problem 3.58

// subq	%rdx, %rsi
// imulq	%rsi, %rdi
// salq	$63, %rsi
// sarq	$63, %rsi
// movq	%rdi, %rax
// xorq	%rsi, %rax
long decode2(long x, long y, long z) {
  long t1 = y - z;
  long t2 = x * t1;
  t1 <<= 63;
  t1 >>= 63;
  return t1 ^ t2;
}
