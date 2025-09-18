#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define max(x, y) ((x) > (y) ? (x) : (y))

char *string_A;
char *string_B;
char *unique_chars_C;
int c_len;
int **DP_Results;

int get_index_of_character(char *str, char x, int len) {
    for (int i = 0; i < len; i++) {
        if (str[i] == x) {
            return i;
        }
    }
    return -1;
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
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_total_time = MPI_Wtime(); // Start time for total runtime

    if (argc != 2) {
        if (rank == 0) {
            printf("Error: Please provide the input file as an argument!\n");
        }
        MPI_Finalize();
        return 0;
    }

    FILE *fp;
    int len_a, len_b;

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        if (rank == 0) {
            printf("Error opening the file.\n");
        }
        MPI_Finalize();
        return 0;
    }

    fscanf(fp, "%d %d %d", &len_a, &len_b, &c_len);

    string_A = (char *)malloc((len_a + 1) * sizeof(char));
    string_B = (char *)malloc((len_b + 1) * sizeof(char));
    unique_chars_C = (char *)malloc((c_len + 1) * sizeof(char));

    fscanf(fp, "%s %s %s", string_A, string_B, unique_chars_C);

    DP_Results = (int **)malloc((len_a + 1) * sizeof(int *));
    for (int k = 0; k < len_a + 1; k++) {
        DP_Results[k] = (int *)calloc((len_b + 1), sizeof(int));
    }

    fclose(fp);

    double start_comm_time = MPI_Wtime(); // Start time for communication

    MPI_Bcast(&len_a, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&len_b, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&c_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    string_A = (char *)realloc(string_A, (len_a + 1) * sizeof(char));
    string_B = (char *)realloc(string_B, (len_b + 1) * sizeof(char));
    unique_chars_C = (char *)realloc(unique_chars_C, (c_len + 1) * sizeof(char));

    MPI_Bcast(string_A, len_a + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(string_B, len_b + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(unique_chars_C, c_len + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    DP_Results = (int **)realloc(DP_Results, (len_a + 1) * sizeof(int *));
    for (int k = 0; k < len_a + 1; k++) {
        DP_Results[k] = (int *)realloc(DP_Results[k], (len_b + 1) * sizeof(int));
    }

    double stop_comm_time = MPI_Wtime(); // Stop time for communication

    double start_comp_time = MPI_Wtime(); // Start time for computation

    int local_lcs_result = lcs(DP_Results, string_A, string_B, len_a, len_b);

    double stop_comp_time = MPI_Wtime(); // Stop time for computation

    int *all_lcs_results = NULL;

    if (rank == 0) {
        all_lcs_results = (int *)malloc(size * sizeof(int));
    }

    MPI_Gather(&local_lcs_result, 1, MPI_INT, all_lcs_results, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int global_lcs_result = 0;
        for (int i = 0; i < size; i++) {
            if (all_lcs_results[i] > global_lcs_result) {
                global_lcs_result = all_lcs_results[i];
            }
        }

        printf("Communication time: %lf seconds\n", stop_comm_time - start_comm_time);
        printf("Computation time: %lf seconds\n", stop_comp_time - start_comp_time);
        printf("Total runtime: %lf seconds\n", MPI_Wtime() - start_total_time);
        printf("LCS computation result: %d\n", global_lcs_result);
    }

    free(string_A);
    free(string_B);
    free(unique_chars_C);

    for (int k = 0; k < len_a + 1; k++) {
        free(DP_Results[k]);
    }
    free(DP_Results);

    if (rank == 0) {
        free(all_lcs_results);
    }

    MPI_Finalize();
    return 0;
}

