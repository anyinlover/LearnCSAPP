/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans3232(int M, int N, int A[N][M], int B[M][N]);
void trans6464(int M, int N, int A[N][M], int B[M][N]);
void trans6167(int M, int N, int A[N][M], int B[M][N]);

void trans3232(int M, int N, int A[N][M], int B[M][N]) {
  int i, k;
  int a0, a1, a2, a3, a4, a5, a6, a7;
  int jl = 8 * (M / 8);
  for (k = 0; k < jl; k += 8) {
    for (i = 0; i < N; i++) {
      a0 = A[i][k];
      a1 = A[i][k + 1];
      a2 = A[i][k + 2];
      a3 = A[i][k + 3];
      a4 = A[i][k + 4];
      a5 = A[i][k + 5];
      a6 = A[i][k + 6];
      a7 = A[i][k + 7];
      B[k][i] = a0;
      B[k + 1][i] = a1;
      B[k + 2][i] = a2;
      B[k + 3][i] = a3;
      B[k + 4][i] = a4;
      B[k + 5][i] = a5;
      B[k + 6][i] = a6;
      B[k + 7][i] = a7;
    }
  }
}

void trans6464(int M, int N, int A[N][M], int B[M][N]) {
  int k, i, j;
  int a0, a1, a2, a3;
  for (j = 0; j < M; j += 8) {

    for (k = 0; k < 4; k++) {
      B[j][(j + k + 8) % M] = A[j + k][j];
      B[j + 1][(j + k + 8) % M] = A[j + k][j + 1];
      B[j + 2][(j + k + 8) % M] = A[j + k][j + 2];
      B[j + 3][(j + k + 8) % M] = A[j + k][j + 3];
      B[j][(j + k + 12) % M] = A[j + k][j + 4];
      B[j + 1][(j + k + 12) % M] = A[j + k][j + 5];
      B[j + 2][(j + k + 12) % M] = A[j + k][j + 6];
      B[j + 3][(j + k + 12) % M] = A[j + k][j + 7];
      B[j][(j + k + 16) % M] = A[j + k + 4][j];
      B[j + 1][(j + k + 16) % M] = A[j + k + 4][j + 1];
      B[j + 2][(j + k + 16) % M] = A[j + k + 4][j + 2];
      B[j + 3][(j + k + 16) % M] = A[j + k + 4][j + 3];
      B[j][(j + k + 20) % M] = A[j + k + 4][j + 4];
      B[j + 1][(j + k + 20) % M] = A[j + k + 4][j + 5];
      B[j + 2][(j + k + 20) % M] = A[j + k + 4][j + 6];
      B[j + 3][(j + k + 20) % M] = A[j + k + 4][j + 7];
    }
    for (k = 0; k < 4; k++) {
      B[j + k][j] = B[j + k][(j + 8) % M];
      B[j + k][j + 1] = B[j + k][(j + 9) % M];
      B[j + k][j + 2] = B[j + k][(j + 10) % M];
      B[j + k][j + 3] = B[j + k][(j + 11) % M];
      B[j + k + 4][j] = B[j + k][(j + 12) % M];
      B[j + k + 4][j + 1] = B[j + k][(j + 13) % M];
      B[j + k + 4][j + 2] = B[j + k][(j + 14) % M];
      B[j + k + 4][j + 3] = B[j + k][(j + 15) % M];
      B[j + k][j + 4] = B[j + k][(j + 16) % M];
      B[j + k][j + 5] = B[j + k][(j + 17) % M];
      B[j + k][j + 6] = B[j + k][(j + 18) % M];
      B[j + k][j + 7] = B[j + k][(j + 19) % M];
      B[j + k + 4][j + 4] = B[j + k][(j + 20) % M];
      B[j + k + 4][j + 5] = B[j + k][(j + 21) % M];
      B[j + k + 4][j + 6] = B[j + k][(j + 22) % M];
      B[j + k + 4][j + 7] = B[j + k][(j + 23) % M];
    }

    for (i = 0; i < N; i += 8) {
      if (i == j)
        continue;
      for (k = 0; k < 4; k++) {
        B[j][i + k] = A[i + k][j];
        B[j + 1][i + k] = A[i + k][j + 1];
        B[j + 2][i + k] = A[i + k][j + 2];
        B[j + 3][i + k] = A[i + k][j + 3];
        B[j][i + k + 4] = A[i + k][j + 4];
        B[j + 1][i + k + 4] = A[i + k][j + 5];
        B[j + 2][i + k + 4] = A[i + k][j + 6];
        B[j + 3][i + k + 4] = A[i + k][j + 7];
      }
      for (k = 0; k < 4; k++) {
        a0 = B[j + k][i + 4];
        a1 = B[j + k][i + 5];
        a2 = B[j + k][i + 6];
        a3 = B[j + k][i + 7];
        B[j + k][i + 4] = A[i + 4][j + k];
        B[j + k][i + 5] = A[i + 5][j + k];
        B[j + k][i + 6] = A[i + 6][j + k];
        B[j + k][i + 7] = A[i + 7][j + k];
        B[j + k + 4][i] = a0;
        B[j + k + 4][i + 1] = a1;
        B[j + k + 4][i + 2] = a2;
        B[j + k + 4][i + 3] = a3;
        B[j + k + 4][i + 4] = A[i + 4][j + k + 4];
        B[j + k + 4][i + 5] = A[i + 5][j + k + 4];
        B[j + k + 4][i + 6] = A[i + 6][j + k + 4];
        B[j + k + 4][i + 7] = A[i + 7][j + k + 4];
      }
    }
  }
}

void trans6167(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, k, g;

  for (i = 0; i < N; i += 17) {
    for (j = 0; j < M; j += 17) {
      for (k = i; k < i + 17 && k < N; ++k) {
        for (g = j; g < j + 17 && g < M; ++g) {
          B[g][k] = A[k][g];
        }
      }
    }
  }
}

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32 && N == 32) {
    trans3232(M, N, A, B);
  } else if (M == 64 && N == 64) {
    trans6464(M, N, A, B);
  } else {
    trans6167(M, N, A, B);
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
void trans(int M, int N, int A[N][M], int B[M][N])
{
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
void registerFunctions()
{
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
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
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

