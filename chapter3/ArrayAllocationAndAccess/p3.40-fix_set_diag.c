// 2022/7/5
// zhangzhong
// Practice Problem 3.40
#define N 16
typedef int fix_matrix[N][N];

void fix_set_diag(fix_matrix A, int val) {
  long i;
  for (i = 0; i < N; i++)
    A[i][i] = val;
}

// -O
void fix_set_diag_opt(fix_matrix A, int val) {
  int *Aptr = &A[0][0];
  int *Aend = &A[N][N];
  do {
    *Aptr = val;
    Aptr += N + 1;
  } while (Aptr != Aend);
}
