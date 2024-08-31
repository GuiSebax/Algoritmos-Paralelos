#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int *vetor;
    int valor;
    int inicio;
    int fim;
    int resultado;
} argumentos;


void *encontra_valor_thread(void *ptr) {
    argumentos *args = (argumentos *) ptr;
    for(int i = args->inicio; i < args->fim; i++){
        if(args->vetor[i] == args->valor){
            args->resultado = i;
            return NULL;
        }
    }
    args->resultado = -1;
    return NULL;
}

int main(void){

    int numthreads;
    printf("Digite o numero de threads: ");
    scanf("%d", &numthreads);
    
    int tam;
    printf("Digite o tamanho do vetor: ");
    scanf("%d", &tam);

    pthread_t threads[numthreads];
    argumentos args[numthreads];

    int vetor[tam];
    srand(time(NULL));
    for(int i = 0; i < tam; i++){
        vetor[i] = rand() % 100;
    }

    for(int i = 0; i < tam; i++){
        printf("%d ", vetor[i]);
    }

    printf("\n");

    int valor;
    printf("Digite o valor a ser procurado: ");
    scanf("%d", &valor);

    int divisao = tam / numthreads;
    int resto = tam % numthreads;

    for(int i = 0; i < numthreads; i++) {
        args[i].vetor = vetor;
        args[i].valor = valor;
        args[i].inicio = i * divisao;
        args[i].fim = (i + 1) * divisao;
        if(i == numthreads - 1) {
            args[i].fim += resto;
        }
        args[i].resultado = -1;
        pthread_create(&threads[i], NULL, encontra_valor_thread, &args[i]);
    }

    int encontrado = -1;   
    for(int i = 0; i < numthreads; i++) {
        pthread_join(threads[i], NULL);
        if(args[i].resultado != -1) {
            encontrado = args[i].resultado;
        }
    }

    if(encontrado != -1) {
        printf("Valor encontrado na posicao %d\n", encontrado);
    } else
        printf("Valor nao encontrado\n");
    
}