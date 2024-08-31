/*

Exemplo 04-02: implementa desafio 02 do exemplo 03: testar 2 threads calculando o
n-ésimo termo da sequencia de Fibonacci em conjunto (cooperada).


===========
Experimento
===========

Digite o codigo em C a seguir em um editor de texto e salve o arquivo com o seguinte
nome:

exemplo04.c

Depois, compile o programa em linha de comando da seguinte forma:

cc -lpthread exemplo04.c -o exemplo4  {ou use -pthread ao inves de -lpthread}

Corrija os erros de compilacao. Quando a compilacao for bem sucedida, o arquivo
executavel "exemplo04" serah criado.

Entao, execute o executavel com a seguinte linha de comando:

./exemplo04

*/

// Guilherme Frare clemente RA: 124349

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define maxElem 50
#define MAX_TAM 10

void *fibonacci(void *ptr);

int fib[maxElem], n, num_thread;

void main(void)
{
     pthread_t threads[MAX_TAM];
     int threadIDS[MAX_TAM];
     int iret[MAX_TAM];

     printf("\nDigite o valor de n > 0 = ");
     scanf("%d", &n);

     while (n < 1) {
        printf("\nDigite o valor de n > 0 = ");
        scanf("%d", &n);
     }

     printf("\nDigite o numero de threads (de 1 a %d) = ", MAX_TAM);
     scanf("%d", &num_thread);

     while(num_thread < 1 || num_thread > MAX_TAM){
        printf("\nNumero invalido, digite um numero de thread (de 1 a %d) = ", MAX_TAM);
        scanf("%d", &num_thread);

     }

     for(int i = 0; i < n; i++)
        fib[i] = 0;
    
    fib[0] = 1;
    fib[1] = 1;

    for(int i = 0; i < num_thread; i++)
    {
        threadIDS[i] = i + 1;
        iret[i] = pthread_create(&threads[i], NULL, fibonacci, (void *)&threadIDS[i]);
    }

    for(int i = 0; i < num_thread; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nSequencia de Fibonacci: \n");
    for(int i = 0; i < n; i++)
    {
        printf("%d, ", fib[i]);
    }

    printf("\n%d-ésimo termo da sequencia é : %d\n", n, fib[n - 1]);

    exit(0);

}

void *fibonacci(void *ptr)
{    int *id, pos;

     id = (int *) ptr;

     printf("\nThread %d iniciada!\n", *id);

     pos=(*id)-1;

     while (pos<n-1)
     {
        pos=pos+2;
        while (fib[pos-1]==0);
        fib[pos]=fib[pos-1]+fib[pos-2];
     }

}

