// ajustar este codigo para calcular multiplicacao matricial com multi-
// plas threads, sendo Cmxp=Amxn*Bnxp. As matrizes A e B devem ser geradas aleatori-
// amente e a matriz C deve ser calculada. Todas as matrizes devem ser mostradas no
// video. Utilize uma thread para cada produto escalar linhaxcoluna */

// Guilherme Frare Clemente RA:124349

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define MAX 10

typedef struct
{
    int linha;
    int coluna;
    int (*A)[MAX];
    int (*B)[MAX];
    int (*C)[MAX];
    int n;
} MatrizData;

void geraMatriz(char *tipo, int mat[MAX][MAX], int linhas, int colunas)
{

    printf("\n%s: \n", tipo);
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            mat[i][j] = rand() % 10;
            printf("%4d", mat[i][j]);
        }
        printf("\n");
    }
}

void *calculaElemento(void *ptr)
{
    MatrizData *data = (MatrizData *)ptr;
    printf("Thread inciciada  para calcular C[%d][%d]\n", data->linha, data->coluna);
    int soma = 0;
    for (int k = 0; k < data->n; k++)
    {
        soma += data->A[data->linha][k] * data->B[k][data->coluna];
    }

    data->C[data->linha][data->coluna] = soma;

    printf("Thread finalizada para calcular C[%d][%d] com valor %d\n", data->linha, data->coluna, soma);

    return NULL;
}
int main(void)
{

    int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];
    int m, n, p;

    srand(time(NULL));

    printf("\nDigite as dimensoes das  matrizes (m n p) onde Cmmxp = Amxn * Bnxp: \n");
    scanf("%d %d %d", &m, &n, &p);

    geraMatriz("A", A, m, n);
    geraMatriz("B", B, n, p);

    pthread_t threads[MAX][MAX];
    MatrizData data[MAX][MAX];

    // Criando threads  para calcular cada elemento da matriz C
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            data[i][j].linha = i;
            data[i][j].coluna = j;
            data[i][j].A = A;
            data[i][j].B = B;
            data[i][j].C = C;
            data[i][j].n = n;
            pthread_create(&threads[i][j], NULL, calculaElemento, &data[i][j]);
        }
    }

    // Esperando threads terminarem
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            pthread_join(threads[i][j], NULL);
        }
    }

    printf("\nMatriz C: \n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            printf("%4d", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
