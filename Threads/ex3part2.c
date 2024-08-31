#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Guilherme Frare clemente
// Ra: 124349

#define max_fib 100


void *fibonacci(void *ptr);

struct tipoPack {
   char mensagem[16];
   int n, fib[max_fib];
   int stepIndex;
};

typedef struct tipoPack tipoPack;

void main(void)
{
     pthread_t thread1, thread2;

     int  iret1, iret2;

     tipoPack pack1, pack2;

     strcpy(pack1.mensagem,"\nSou a thread 1");

     strcpy(pack2.mensagem,"\nSou a thread 2");

     printf("\nDigite o n-ésimo valor a ser mostrado na sequência de Fibonacci > ");
     scanf("%d",&(pack1.n));
     pack2.n = pack1.n;

    //  Inicializar com os valores de fibonacci padroes
    pack1.fib[0] = 0;
    pack1.fib[1] = 1;
    pack2.fib[0] = 0;
    pack2.fib[1] = 1;

    // Setar as threads para o indice especifico
    pack1.stepIndex = 2;
    pack2.stepIndex = 3;

     iret1 = pthread_create(&thread1, NULL, fibonacci, (void*) &pack1);
     iret2 = pthread_create(&thread2, NULL, fibonacci, (void*) &pack2);

     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);

     for(int i = 2; i <= pack1.n; i++) {
        if(pack1.fib[i] == 0 && pack2.fib[i] != 0)
            pack1.fib[i] = pack2.fib[i];
        else if(pack2.fib[i] == 0 && pack1.fib[i] != 0)
            pack2.fib[i] = pack1.fib[i];
     }

     

     printf("\no %d-esimo elemento da sequencia de Fibonacci e: %d\n", pack1.n, pack1.fib[pack1.n]);
     printf("\nSequencia\n");
     for(int i = 0; i <= pack1.n; i++){
        printf("%d ", pack1.fib[i]);
     }

     printf("\n");
     exit(0);
}

void *fibonacci(void *ptr) {
    tipoPack *pack = (tipoPack *) ptr;
    printf("%s: Vou calcular o n-esimo elemetno da sequencia de fibonacci\n", pack->mensagem);

    for(int i = pack->stepIndex; i <= pack->n; i++) {
        if(i == 0 || i == 1) continue;
        pack->fib[i] = pack->fib[i - 1] + pack->fib[i - 2];
    }

}
