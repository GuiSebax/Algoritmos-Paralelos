#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Guilherme Frare clemente
// Ra: 124349

void *fatorial(void *ptr);

struct tipoPack {
   char mensagem[16];
   int tam, fatorial;
};

typedef struct tipoPack tipoPack;

void main(void)
{
     pthread_t thread1, thread2;

     int  iret1, iret2;

     tipoPack pack1, pack2;

     strcpy(pack1.mensagem,"\nSou a thread 1");
     pack1.fatorial = 1;

     strcpy(pack2.mensagem,"\nSou a thread 2");
     pack2.fatorial = 1;

     printf("\nDigite o valor para o primeiro fatorial  > ");
     scanf("%d",&(pack1.tam));

     printf("\nDigite o valor para o segundo fatorial > ");
     scanf("%d",&(pack2.tam));

     iret1 = pthread_create(&thread1, NULL, fatorial, (void*) &pack1);

     iret2 = pthread_create(&thread2, NULL, fatorial, (void*) &pack2);

     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);

     printf("\nThread 1 fatorial: %d\n",pack1.fatorial);
     printf("\nThread 2 fatorial: %d\n",pack2.fatorial);
     exit(0);
}

void *fatorial(void *ptr) 
{
    int i;
    tipoPack *pack;
    pack = (tipoPack *) ptr;

    printf("%s: vou realizar o fatorial", pack->mensagem);

    for(int i = 1; i <= pack->tam; i++)
        pack->fatorial = pack->fatorial * i;

}

