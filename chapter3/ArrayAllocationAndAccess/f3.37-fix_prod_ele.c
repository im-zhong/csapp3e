// 2022/7/5
// zhangzhong
// Figure 3.37 Original and optimized code to compute element i,k of matrix
// product for fixed-length arrays

#define N 16
typedef int fix_matrix[N][N];

// compute i,k of fixed matrix product
int fix_prod_ele(fix_matrix A, fix_matrix B, long i, long k) {
  long j = 0;
  int result = 0;

  for (j = 0; j < N; j++)
    result += A[i][j] * B[j][k];

  return result;
}

// -O
int fix_prod_ele_opt(fix_matrix A, fix_matrix B, long i, long k) {
  int *Aptr = &A[i][0]; // points to elements in row i of A
  int *Bptr = &B[0][k]; // points to elements in col k of B
  int *Bend = &B[N][k]; // marks stoppint point for Bptr

  int result = 0;
  do {                       // no need for initial test
    result += *Aptr * *Bptr; // result += A[i][j] * B[j][k];
    Aptr++;                  // move Aptr to next column
    Bptr += N;               // move Bptr to next row
  } while (Bptr != Bend);    // test for stoppint point
  return result;
}
