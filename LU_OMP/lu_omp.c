#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void printMatrix(double **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int size;

    printf("Enter the size of the square matrix: ");
    scanf("%d", &size);

    double **A = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        A[i] = (double *)malloc(size * sizeof(double));
    }

    double **L = (double **)malloc(size * sizeof(double *));
    double **U = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        L[i] = (double *)calloc(size, sizeof(double));
        U[i] = (double *)calloc(size, sizeof(double));
    }

    printf("Enter the elements of the %dx%d matrix:\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%lf", &A[i][j]);
        }
    }

    int i, j, k;
double startTime = omp_get_wtime();
    // LU decomposition
    for (i = 0; i < size; i++) {
        #pragma omp parallel for private(j, k) shared(A, L, U) schedule(static)
        for (j = i; j < size; j++) {
            U[i][j] = A[i][j];
            for (k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        #pragma omp parallel for private(j, k) shared(A, L, U) schedule(static)
        for (j = i; j < size; j++) {
            if (i == j)
                L[i][i] = 1.0;
            else {
                L[j][i] = A[j][i];
                for (k = 0; k < i; k++) {
                    L[j][i] -= L[j][k] * U[k][i];
                }
                L[j][i] /= U[i][i];
            }
        }
    }
	double endTime = omp_get_wtime();
    double totalTime = endTime - startTime;

    printf("Original Matrix A:\n");
    printMatrix(A, size);

    printf("Lower Triangular Matrix L:\n");
    printMatrix(L, size);

    printf("Upper Triangular Matrix U:\n");
    printMatrix(U, size);

   printf("Total runtime: %f seconds\n", totalTime);

    for (int i = 0; i < size; i++) {
        free(A[i]);
        free(L[i]);
        free(U[i]);
    }
    free(A);
    free(L);
    free(U);

    return 0;
}
