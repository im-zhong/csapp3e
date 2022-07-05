// 2022/7/5
// zhangzhong
// Practice Problem 3.38

#define M 5
#define N 7

long P[M][N];
long Q[N][M];

// rdi = i, rsi = j
// leaq	0(,%rdi,8), %rdx		// rdx = 8i
// subq	%rdi, %rdx				// rdx = 7i
// addq	%rsi, %rdx				// rdx = 7i + j
// leaq	(%rsi,%rsi,4), %rax		// rax = 5j
// addq	%rdi, %rax				// rax = 5j + i
// leaq	Q(%rip), %rcx			// rcx = &Q
// movq	(%rcx,%rax,8), %rax		// rax = *(Q + 8(5j + i))
// leaq	P(%rip), %rcx			// rcx = &P
// addq	(%rcx,%rdx,8), %rax		// rax += *(P + 8(7i + j))
long sum_element(long i, long j) {
  // P[i][j] = *(P + 8(Ni + j))
  // Q[j][i] = *(Q + 8(Mj + i))
  return P[i][j] + Q[j][i];
}
