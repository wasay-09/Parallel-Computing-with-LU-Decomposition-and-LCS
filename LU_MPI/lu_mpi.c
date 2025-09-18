#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void printMatrix(double **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int size, rank;
double startTime, endTime, totalTime;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
    startTime = MPI_Wtime();
        FILE *file;
        file = fopen("matrix.txt", "r");

        if (file == NULL) {
            printf("Error opening the file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fscanf(file, "%d", &size);

        double **A = (double **)malloc(size * sizeof(double *));
        for (int i = 0; i < size; i++) {
            A[i] = (double *)malloc(size * sizeof(double));
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                fscanf(file, "%lf", &A[i][j]);
            }
        }

        fclose(file);

        MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = 1; i < size; i++) {
            MPI_Send(&(A[i][0]), size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }

        double **L = (double **)malloc(size * sizeof(double *));
        double **U = (double **)malloc(size * sizeof(double *));
        for (int i = 0; i < size; i++) {
            L[i] = (double *)calloc(size, sizeof(double));
            U[i] = (double *)calloc(size, sizeof(double));
        }

        for (int i = 0; i < size; i++) {
            for (int j = i; j < size; j++) {
                U[i][j] = A[i][j];
                for (int k = 0; k < i; k++) {
                    U[i][j] -= L[i][k] * U[k][j];
                }
            }

            for (int j = i; j < size; j++) {
                if (i == j)
                    L[i][i] = 1.0;
                else {
                    L[j][i] = A[j][i];
                    for (int k = 0; k < i; k++) {
                        L[j][i] -= L[j][k] * U[k][i];
                    }
                    L[j][i] /= U[i][i];
                }
            }
        }

        printf("Original Matrix A:\n");
        printMatrix(A, size);

        printf("Lower Triangular Matrix L:\n");
        printMatrix(L, size);

        printf("Upper Triangular Matrix U:\n");
        printMatrix(U, size);
        
	endTime = MPI_Wtime();
        totalTime = endTime - startTime;
        printf("Total runtime: %f seconds\n", totalTime);
        for (int i = 0; i < size; i++) {
            free(A[i]);
            free(L[i]);
            free(U[i]);
        }
        free(A);
        free(L);
        free(U);
    } else {
        MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        double **A = (double **)malloc(size * sizeof(double *));
        for (int i = 0; i < size; i++) {
            A[i] = (double *)malloc(size * sizeof(double));
        }

        MPI_Recv(&(A[rank][0]), size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        for (int i = 0; i < size; i++) {
            free(A[i]);
        }
        free(A);
    }

    MPI_Finalize();

    return 0;
}
