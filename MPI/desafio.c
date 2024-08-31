#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_NUM 1000

typedef int **Matrix;

Matrix allocateMatrix(int rows, int cols);
void freeMatrix(Matrix mat, int rows);
void generateMatrix(Matrix mat, int rows, int cols);
void printMatrix(const char *name, Matrix mat, int rows, int cols);

int main(int argc, char *argv[])
{
    int id, np, rows, cols, i, j, k;
    int *rowA, *colB, result;
    Matrix A, B, C;
    MPI_Status st;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    if (id == 0)
    {
        printf("Digite o numero de linhas e colunas das matrizes:\n");
        printf("Linhas de A e C (e colunas de B): ");
        scanf("%d", &rows);
        printf("Colunas de A e C (e linhas de B): ");
        scanf("%d", &cols);

        A = allocateMatrix(rows, cols);
        B = allocateMatrix(cols, rows); // Para multiplicação, B deve ter dimensões transpostas
        C = allocateMatrix(rows, rows); // Resultado da multiplicação

        // Inicializa o gerador de números aleatórios uma vez
        srand(time(NULL));

        generateMatrix(A, rows, cols);
        generateMatrix(B, cols, rows);

        printMatrix("Matriz A", A, rows, cols);
        printMatrix("Matriz B", B, cols, rows);

        for (i = 1; i < np; i++)
        {
            MPI_Send(&rows, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&cols, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
        MPI_Recv(&cols, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
    }

    rowA = (int *)malloc(cols * sizeof(int));
    colB = (int *)malloc(cols * sizeof(int));

    if (id == 0)
    {
        for (i = 1; i < np; i++)
        {
            MPI_Send(A[i - 1], cols, MPI_INT, i, 0, MPI_COMM_WORLD);
            for (j = 0; j < cols; j++)
            {
                for (k = 0; k < rows; k++)
                {
                    colB[k] = B[j][k];
                }
                MPI_Send(colB, rows, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        for (i = 0; i < cols; i++)
        {
            for (j = 0; j < rows; j++)
            {
                C[i][j] = 0;
                for (k = 0; k < cols; k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        for (i = 1; i < np; i++)
        {
            MPI_Recv(&result, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &st);
            C[i - 1][i - 1] = result;
        }

        printMatrix("Matriz Resultado", C, rows, rows);
    }
    else
    {
        MPI_Recv(rowA, cols, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
        for (i = 0; i < rows; i++)
        {
            MPI_Recv(colB, rows, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
            result = 0;
            for (j = 0; j < cols; j++)
            {
                result += rowA[j] * colB[j];
            }
            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    free(rowA);
    free(colB);

    if (id == 0)
    {
        freeMatrix(A, rows);
        freeMatrix(B, cols);
        freeMatrix(C, rows);
    }

    MPI_Finalize();
    return 0;
}

Matrix allocateMatrix(int rows, int cols)
{
    Matrix mat = (Matrix)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        mat[i] = (int *)malloc(cols * sizeof(int));
    }
    return mat;
}

void freeMatrix(Matrix mat, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

void generateMatrix(Matrix mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            mat[i][j] = rand() % MAX_NUM;
        }
    }
}

void printMatrix(const char *name, Matrix mat, int rows, int cols)
{
    printf("\n%s:\n", name);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}
