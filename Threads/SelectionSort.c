#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Guilherme Frare Clemente
// Ra:124349

/*
Selection Sort:

Algoritmo que ordena um vetor com n elementos inteiros usando
2 threads.

*/

typedef struct
{
    int *vet;
    int tam;

} TipoDado;

void selectionSort(int vet[], int tam)
{

    for (int j = 0; j < tam / 2; j++)
    {

        int menorIndiceateAgora = j;
        for (int i = j; i < tam; i++)
        {
            if (vet[menorIndiceateAgora] > vet[i])
            {
                menorIndiceateAgora = i;
            }
        }

        int aux = vet[j];
        vet[j] = vet[menorIndiceateAgora];
        vet[menorIndiceateAgora] = aux;
    }
}

void selectionSort2(int vet[], int tam)
{

    for (int i = tam; i >= tam / 2; i--)
    {

        int maiorIndiceateAgora = 0;

        for (int j = 0; j < i; j++)
        {
            if (vet[maiorIndiceateAgora] < vet[j])
            {
                maiorIndiceateAgora = j;
            }
        }

        int aux = vet[i - 1];
        vet[i - 1] = vet[maiorIndiceateAgora];
        vet[maiorIndiceateAgora] = aux;
    }
}

void *thread10(void *arg)
{

    TipoDado *dado = (TipoDado *)arg;
    selectionSort(dado->vet, dado->tam);

    return NULL;
}

void *thread20(void *arg)
{
    TipoDado *dado = (TipoDado *)arg;
    selectionSort2(dado->vet, dado->tam);

    return NULL;
}

int main(void)
{

    int n;
    printf("Digite o tamanho do vetor: ");
    scanf("%d", &n);

    int vet[n];
    srand(time(NULL));

    for (int i = 0; i < n; i++)
    {
        vet[i] = rand() % 100;
    }

    for (int i = 0; i < n; i++)
        printf("%d ", vet[i]);

    printf("\n\n");

    pthread_t thread1, thread2;
    TipoDado d1;

    d1.vet = vet;
    d1.tam = n;

    pthread_create(&thread1, NULL, thread10, (void *)&d1);
    pthread_create(&thread2, NULL, thread20, (void *)&d1);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    for (int i = 0; i < n; i++)
        printf("%d ", vet[i]);

    printf("\n");

    return 0;
}
