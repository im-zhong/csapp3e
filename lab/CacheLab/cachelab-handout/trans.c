/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include "cachelab.h"
#include <stdio.h>

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
  // case1: M = N = 32
  // 这样的话得写成四个循环了
  // 分块之后的情况
  // int line[8];
  int m0, m1, m2, m3, m4, m5, m6, m7;
  int m, n, i, j;

  if (M == 32 && N == 32) {
    // int m, n, i, j;
    for (m = 0; m < M / 8; ++m) {
      for (n = 0; n < N / 8; ++n) {
        for (i = 0; i < 8; ++i) {
          // for (j = 0; j < 8; ++j) {
          //   // 在这里先把一行都读出来 放到局部变量里面
          //   line[j] = A[i + 8 * m][j + 8 * n];
          // }
          // 把上面的循环改成八个读取
          m0 = A[i + 8 * m][0 + 8 * n];
          m1 = A[i + 8 * m][1 + 8 * n];
          m2 = A[i + 8 * m][2 + 8 * n];
          m3 = A[i + 8 * m][3 + 8 * n];
          m4 = A[i + 8 * m][4 + 8 * n];
          m5 = A[i + 8 * m][5 + 8 * n];
          m6 = A[i + 8 * m][6 + 8 * n];
          m7 = A[i + 8 * m][7 + 8 * n];

          // for (j = 0; j < 8; ++j) {
          //   B[j + 8 * n][i + 8 * m] = line[j];
          // }
          B[0 + 8 * n][i + 8 * m] = m0;
          B[1 + 8 * n][i + 8 * m] = m1;
          B[2 + 8 * n][i + 8 * m] = m2;
          B[3 + 8 * n][i + 8 * m] = m3;
          B[4 + 8 * n][i + 8 * m] = m4;
          B[5 + 8 * n][i + 8 * m] = m5;
          B[6 + 8 * n][i + 8 * m] = m6;
          B[7 + 8 * n][i + 8 * m] = m7;
          // for (j = 0; j < 8; ++j) {
          //   B[j + 8 * n][i + 8 * m] = A[i + 8 * m][j + 8 * n];
          // }
        }
      }
    }
  } else if (M == 64 && N == 64) {
    // case2: 64 x 64
    // 还是按照8x8的块来处理
    // 尽可能将块内部的miss降到20以内 就可以了
    // 我这里让m+=8 不久可以直接用m代替i和j的功能了吗
    // 这样就空出来两个变量可以用了
    for (m = 0; m < M; m += 8) {
      for (n = 0; n < M; n += 8) {
        for (i = m; i < m + 4; ++i) {
          m0 = A[i][n + 0];
          m1 = A[i][n + 1];
          m2 = A[i][n + 2];
          m3 = A[i][n + 3];
          m4 = A[i][n + 4];
          m5 = A[i][n + 5];
          m6 = A[i][n + 6];
          m7 = A[i][n + 7];

          B[n + 0][i] = m0;
          B[n + 1][i] = m1;
          B[n + 2][i] = m2;
          B[n + 3][i] = m3;

          B[n + 0][i + 4] = m4;
          B[n + 1][i + 4] = m5;
          B[n + 2][i + 4] = m6;
          B[n + 3][i + 4] = m7;
        }

        for (j = n; j < n + 4; ++j) {
          m0 = A[i + 0][j];
          m1 = A[i + 1][j];
          m2 = A[i + 2][j];
          m3 = A[i + 3][j];

          m4 = B[j][m + 4];
          m5 = B[j][m + 5];
          m6 = B[j][m + 6];
          m7 = B[j][m + 7];

          B[j][m + 4] = m0;
          B[j][m + 5] = m1;
          B[j][m + 6] = m2;
          B[j][m + 7] = m3;

          B[j + 4][m + 0] = m4;
          B[j + 4][m + 1] = m5;
          B[j + 4][m + 2] = m6;
          B[j + 4][m + 3] = m7;
        }

        for (j = n + 4; i < m + 8; ++i) {
          m0 = A[i][j + 0];
          m1 = A[i][j + 1];
          m2 = A[i][j + 2];
          m3 = A[i][j + 3];

          B[j + 0][i] = m0;
          B[j + 1][i] = m1;
          B[j + 2][i] = m2;
          B[j + 3][i] = m3;
        }
      }
    }
  } else if (M == 61 && N == 67) {
    // case3: M=61, N=67
    // const int k = 9;
    // const int l = 8;
    for (n = 0; n < 9; ++n) {
      for (m = 0; m < 8; ++m) {
        for (int i = 0; i < 8; ++i) {
          if (n == 8 && m != 7) {
            if (i < 3) {
              m0 = A[i + 8 * n][0 + 8 * m];
              m1 = A[i + 8 * n][1 + 8 * m];
              m2 = A[i + 8 * n][2 + 8 * m];
              m3 = A[i + 8 * n][3 + 8 * m];
              m4 = A[i + 8 * n][4 + 8 * m];
              m5 = A[i + 8 * n][5 + 8 * m];
              m6 = A[i + 8 * n][6 + 8 * m];
              m7 = A[i + 8 * n][7 + 8 * m];

              B[0 + 8 * m][i + 8 * n] = m0;
              B[1 + 8 * m][i + 8 * n] = m1;
              B[2 + 8 * m][i + 8 * n] = m2;
              B[3 + 8 * m][i + 8 * n] = m3;
              B[4 + 8 * m][i + 8 * n] = m4;
              B[5 + 8 * m][i + 8 * n] = m5;
              B[6 + 8 * m][i + 8 * n] = m6;
              B[7 + 8 * m][i + 8 * n] = m7;
            }
          } else if (n != 8 && m == 7) {
            m0 = A[i + 8 * n][0 + 8 * m];
            m1 = A[i + 8 * n][1 + 8 * m];
            m2 = A[i + 8 * n][2 + 8 * m];
            m3 = A[i + 8 * n][3 + 8 * m];
            m4 = A[i + 8 * n][4 + 8 * m];

            B[0 + 8 * m][i + 8 * n] = m0;
            B[1 + 8 * m][i + 8 * n] = m1;
            B[2 + 8 * m][i + 8 * n] = m2;
            B[3 + 8 * m][i + 8 * n] = m3;
            B[4 + 8 * m][i + 8 * n] = m4;
          } else if (n == 8 && m == 7) {
            if (i < 3) {
              m0 = A[i + 8 * n][0 + 8 * m];
              m1 = A[i + 8 * n][1 + 8 * m];
              m2 = A[i + 8 * n][2 + 8 * m];
              m3 = A[i + 8 * n][3 + 8 * m];
              m4 = A[i + 8 * n][4 + 8 * m];

              B[0 + 8 * m][i + 8 * n] = m0;
              B[1 + 8 * m][i + 8 * n] = m1;
              B[2 + 8 * m][i + 8 * n] = m2;
              B[3 + 8 * m][i + 8 * n] = m3;
              B[4 + 8 * m][i + 8 * n] = m4;
            }
          } else {
            m0 = A[i + 8 * n][0 + 8 * m];
            m1 = A[i + 8 * n][1 + 8 * m];
            m2 = A[i + 8 * n][2 + 8 * m];
            m3 = A[i + 8 * n][3 + 8 * m];
            m4 = A[i + 8 * n][4 + 8 * m];
            m5 = A[i + 8 * n][5 + 8 * m];
            m6 = A[i + 8 * n][6 + 8 * m];
            m7 = A[i + 8 * n][7 + 8 * m];

            B[0 + 8 * m][i + 8 * n] = m0;
            B[1 + 8 * m][i + 8 * n] = m1;
            B[2 + 8 * m][i + 8 * n] = m2;
            B[3 + 8 * m][i + 8 * n] = m3;
            B[4 + 8 * m][i + 8 * n] = m4;
            B[5 + 8 * m][i + 8 * n] = m5;
            B[6 + 8 * m][i + 8 * n] = m6;
            B[7 + 8 * m][i + 8 * n] = m7;
          }
        }
        // print_matrix(M, N, (int *)B);
      }
    }
  } else {
    // normal case
    for (i = 0; i < N; i++) {
      for (j = 0; j < M; j++) {
        B[j][i] = A[i][j];
      }
    }
  }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, tmp;

  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      tmp = A[i][j];
      B[j][i] = tmp;
    }
  }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
  /* Register your solution function */
  registerTransFunction(transpose_submit, transpose_submit_desc);

  /* Register any additional transpose functions */
  registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
  int i, j;

  for (i = 0; i < N; i++) {
    for (j = 0; j < M; ++j) {
      if (A[i][j] != B[j][i]) {
        return 0;
      }
    }
  }
  return 1;
}
