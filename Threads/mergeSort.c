#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// Guilherme Frare Clemente RA:124349
// MergeSort versão Multithread

#define maxVet 10000
#define maxNumero 10000

typedef int tipoVetor[maxVet];

tipoVetor A;

typedef struct
{
    tipoVetor *V;
    int inicio;
    int fim;
} Args;

int tamA;
int tamanhoMinimoThread; // Variável para armazenar o tamanho mínimo para dividir em threads

void mergeSort(tipoVetor *V, int ini, int fim);

int leTam(void)
{
    int tam;
    do
    {
        printf("\nDigite o tamanho do Vetor = ");
        scanf("%d", &tam);
    } while (tam < 1 || tam > maxVet);
    return tam;
}

void geraVetor(tipoVetor V, int tam)
{
    int i, sinal;
    srand(time(NULL));
    printf("\nGeracao Aleatoria do Vetor\n");
    for (i = 0; i < tam; i++)
    {
        V[i] = rand() % maxNumero;
        sinal = (rand() % 2) * 2 - 1; // se rand%2==0, sinal=-1; se rand%2==1, sinal=+1
        V[i] = sinal * V[i];          // impoe o sinal no numero
    }
}

void mostraVetor(char *id, tipoVetor V, int tam)
{
    int i;
    printf("\nElementos do Vetor %s:\n", id);
    printf("%d", V[0]);
    for (i = 1; i < tam; i++)
    {
        printf(", %d", V[i]);
    }
    printf("\n");
}

void mergeMetades(tipoVetor V, int ini, int meio, int fim)
{
    tipoVetor auxV1, auxV2;
    int tam1 = 0, tam2 = 0, maior, i, i1, i2;

    // copia primeira metade V[ini..meio] para auxV1
    for (i = ini; i <= meio; i++)
    {
        auxV1[tam1] = V[i];
        tam1++;
    }

    // copia segunda metade V[meio+1..fim] para auxV2
    for (i = meio + 1; i <= fim; i++)
    {
        auxV2[tam2] = V[i];
        tam2++;
    }

    // descobre um valor maior que todos do segmento
    if (auxV1[tam1 - 1] > auxV2[tam2 - 1])
        maior = abs(auxV1[tam1 - 1]) + 1;
    else
        maior = abs(auxV2[tam2 - 1]) + 1;

    // insere o valor maior que todos logo apos o ultimo de cada metade
    auxV1[tam1] = maior;
    auxV2[tam2] = maior;

    // avança em cada metade retirando o menor elemento e colocando de volta no segmento
    i1 = 0;
    i2 = 0;
    for (i = ini; i <= fim; i++)
    {
        if (auxV1[i1] < auxV2[i2])
        {
            V[i] = auxV1[i1];
            i1++;
        }
        else
        {
            V[i] = auxV2[i2];
            i2++;
        }
    }
}

void *mergeSortThread(void *ptr)
{
    Args *args = (Args *)ptr;
    printf("Thread working on segment: [%d, %d]\n", args->inicio, args->fim);
    mergeSort(args->V, args->inicio, args->fim);
    printf("Thread completed on segment: [%d, %d]\n", args->inicio, args->fim);
    return NULL;
}

void mergeSort(tipoVetor *V, int ini, int fim)
{
    pthread_t thread1, thread2;
    Args args1, args2;

    if (fim - ini + 1 > tamanhoMinimoThread) // Dividir em threads se o tamanho for maior que o mínimo definido
    {
        int meio = (ini + fim) / 2;

        args1.V = V;
        args1.inicio = ini;
        args1.fim = meio;

        args2.V = V;
        args2.inicio = meio + 1;
        args2.fim = fim;

        printf("Creating thread for segment: [%d, %d]\n", args1.inicio, args1.fim);
        pthread_create(&thread1, NULL, mergeSortThread, &args1);

        printf("Creating thread for segment: [%d, %d]\n", args2.inicio, args2.fim);
        pthread_create(&thread2, NULL, mergeSortThread, &args2);

        pthread_join(thread1, NULL);
        printf("Thread joined for segment: [%d, %d]\n", args1.inicio, args1.fim);

        pthread_join(thread2, NULL);
        printf("Thread joined for segment: [%d, %d]\n", args2.inicio, args2.fim);

        mergeMetades(*V, ini, meio, fim);
    }
    else if (ini < fim) // Caso base da recursão (tamanho igual a 1)
    {
        int meio = (ini + fim) / 2;

        mergeSort(V, ini, meio);
        mergeSort(V, meio + 1, fim);

        mergeMetades(*V, ini, meio, fim);
    }
}

int main(void)
{
    int resp;

    // Pedir ao usuário o tamanho mínimo para dividir em threads
    printf("\nDigite o tamanho mínimo para dividir em threads = ");
    scanf("%d", &tamanhoMinimoThread);

    do
    {
        tamA = leTam();

        geraVetor(A, tamA);

        mostraVetor("A Original", A, tamA);

        printf("\nOrdenando A por Merge Sort ...\n");

        mergeSort(&A, 0, tamA - 1);

        mostraVetor("A Ordenado", A, tamA);

        printf("\nDeseja Nova Execucao, <1>Sim <2>Nao? => ");
        scanf("%d", &resp);

    } while (resp == 1);

    return 0;
}
