#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Guilherme Frare Clemente RA:124349
// K threads que calculam fatorials
#define MAX_TAM 100

void *fatorial(void *ptr);

struct tipoFatData
{
    char nome[10];
    int n, fat;
};

typedef struct tipoFatData tipoFatData;

void main(void)
{
    pthread_t threads[MAX_TAM];
    tipoFatData fat[MAX_TAM];
    int qtd_threads;

    printf("Digite o número de threads necessárias: MAXIMO DE %d\n", MAX_TAM);
    scanf("%d", &qtd_threads);

    for (int i = 0; i < qtd_threads; i++)
    {
        fat[i].fat = 1;
        printf("Digite n > ");
        scanf("%d", &fat[i].n);
        pthread_create(&threads[i], NULL, fatorial, (void *)&fat[i]);
    }

    for (int i = 0; i < qtd_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < qtd_threads; i++)
    {
        printf("Fat(%d) = %d\n", fat[i].n, fat[i].fat);
    }
}

void *fatorial(void *ptr)
{
    int i;

    tipoFatData *fatData;

    fatData = (tipoFatData *)ptr;

    for (i = fatData->n; i > 1; i--)
        (fatData->fat) = (fatData->fat) * i;
}
