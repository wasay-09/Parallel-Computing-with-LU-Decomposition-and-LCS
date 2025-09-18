#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ALPHABET_LENGTH 4
#define max(x, y) ((x) > (y) ? (x) : (y))

char *string_A;
char *string_B;
char *unique_chars_C;
int c_len;
int **P_Matrix;
int **DP_Results;

int get_index_of_character(char *str, char x, int len) {
    for (int i = 0; i < len; i++) {
        if (str[i] == x) {
            return i;
        }
    }
    return -1;
}

void print_matrix(int **x, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d ", x[i][j]);
        }
        printf("\n");
    }
}

void calc_P_matrix_v1(int **P, char *b, int len_b, char *c, int len_c) {
    #pragma omp parallel for
    for (int i = 0; i < len_c; i++) {
        for (int j = 2; j < len_b + 1; j++) {
            if (b[j - 2] == c[i]) {
                P[i][j] = j - 1;
            } else {
                P[i][j] = P[i][j - 1];
            }
        }
    }
}

int lcs_yang_v1(int **DP, int **P, char *A, char *B, char *C, int m, int n, int u) {
    for (int i = 1; i < m + 1; i++) {
        int c_i = get_index_of_character(C, A[i - 1], u);
        #pragma omp parallel for schedule(static)
        for (int j = 0; j < n + 1; j++) {
            if (A[i - 1] == B[j - 1]) {
                DP[i][j] = DP[i - 1][j - 1] + 1;
            } else if (P[c_i][j] == 0) {
                DP[i][j] = max(DP[i - 1][j], 0);
            } else {
                DP[i][j] = max(DP[i - 1][j], DP[i - 1][P[c_i][j] - 1] + 1);
            }
        }
    }
    return DP[m][n];
}

int lcs(int **DP, char *A, char *B, int m, int n) {
    for (int i = 1; i < (m + 1); i++) {
        for (int j = 1; j < (n + 1); j++) {
            if (A[i - 1] == B[j - 1]) {
                DP[i][j] = DP[i - 1][j - 1] + 1;
            } else {
                DP[i][j] = max(DP[i - 1][j], DP[i][j - 1]);
            }
        }
    }
    return DP[m][n];
}

int main(int argc, char *argv[]) {
     if(argc != 2) {
        printf("Error: Please provide the input file as an argument!\n");
        return 0;
    }

    FILE *fp;
    int len_a, len_b;
    double start_time, stop_time;

    fp = fopen(argv[1], "r");
    if(fp == NULL) {
        printf("Error opening the file.\n");
        return 0;
    }

    fscanf(fp, "%d %d %d", &len_a, &len_b, &c_len);

    string_A = (char *)malloc((len_a + 1) * sizeof(char));
    string_B = (char *)malloc((len_b + 1) * sizeof(char));
    //unique_chars_C = (char *)malloc((c_len + 1) * sizeof(char));

    fscanf(fp, "%s %s %s", string_A, string_B, unique_chars_C);

    DP_Results = (int **)malloc((len_a + 1) * sizeof(int *));
    for(int k = 0; k < len_a + 1; k++) {
        DP_Results[k] = (int *)calloc((len_b + 1), sizeof(int));
    }

    P_Matrix = (int **)malloc(c_len * sizeof(int *));
    for(int k = 0; k < c_len; k++) {
        P_Matrix[k] = (int *)calloc((len_b + 1), sizeof(int));
    }

    start_time = omp_get_wtime();
    printf("LCS is: %d\n", lcs(DP_Results, string_A, string_B, len_a, len_b));
    stop_time = omp_get_wtime();
    printf("runtime: %lf seconds\n", stop_time - start_time);

    free(string_A);
    free(string_B);
    free(unique_chars_C);

    for(int k = 0; k < len_a + 1; k++) {
        free(DP_Results[k]);
    }
    free(DP_Results);

    for(int k = 0; k < c_len; k++) {
        free(P_Matrix[k]);
    }
    free(P_Matrix);

    fclose(fp);
    return 0;
}
