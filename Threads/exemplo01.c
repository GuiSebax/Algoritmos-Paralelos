#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Guilherme Frare Clemente - RA:124349

// Atividade 1

// Tamanho do vetor e tamanho do bloco para ser preenchido
#define TAM_VETOR 3000

// Tamanho do vetor
int vet[TAM_VETOR];

void *funcaoImprimeMensagem(void *ptr);
void *funcaopreenchevetor(void *ptr);

void main(void)
{
    // Criando todas as threads
    pthread_t thread1, thread2, thread3;
    char *mensagem1 = "1";
    char *mensagem2 = "2";
    char *mensagem3 = "3";
    int iret1, iret2, iret3;

    /* Cria 2 threads independentes e ambas executarao a mesma funcao
       com argumentos diferentes */

    iret1 = pthread_create(&thread1, NULL, funcaoImprimeMensagem, (void *)mensagem1);
    iret2 = pthread_create(&thread2, NULL, funcaoImprimeMensagem, (void *)mensagem2);
    iret3 = pthread_create(&thread3, NULL, funcaoImprimeMensagem, (void *)mensagem3);

    printf("\nCriacao da thread 1 retornou: %d\n", iret1);
    printf("\nCriacao da thread 2 retornou: %d\n", iret2);
    printf("\nCriacao da thread 3 retornou: %d\n", iret3);

    /* Espera as threads completarem para entao continuar, para nao
       correr o risco de terminar o programa principal antes das
       threads terminarem. O termino do processo causa forcadamente
       o termino de suas tarefas */

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    // Criacao de thread para preencher o vetor
    pthread_t thread_vetor1, thread_vetor2, thread_vetor3;
    iret1 = pthread_create(&thread_vetor1, NULL, funcaopreenchevetor, (void *)mensagem1);
    iret2 = pthread_create(&thread_vetor2, NULL, funcaopreenchevetor, (void *)mensagem2);
    iret3 = pthread_create(&thread_vetor3, NULL, funcaopreenchevetor, (void *)mensagem3);

    printf("\nCriacao da thread para preencher vetor 1 retornou: %d\n", iret1);
    printf("Criacao da thread para preencher vetor 2 retornou: %d\n", iret2);
    printf("Criacao da thread para preencher vetor 3 retornou: %d\n", iret3);

    // Evitar que a execução das thread sejam interrompidas
    pthread_join(thread_vetor1, NULL);
    pthread_join(thread_vetor2, NULL);
    pthread_join(thread_vetor3, NULL);

    // Imprime os valores do vetor preenchido e define contadores para verificar se as regras foram feitas de modo correto
    printf("\nVetor Resultante: ");
    int contador1 = 0;
    int contador2 = 0;
    int contador3 = 0;

    for (int i = 0; i < TAM_VETOR; i++)
    {

        if (vet[i] == 1)
        {
            contador1++;
            printf("%d", vet[i]);
        }
        else if (vet[i] == 2)
        {
            contador2++;
            printf("%d", vet[i]);
        }
        else if (vet[i] == 3)
        {
            contador3++;
            printf("%d", vet[i]);
        }
        else
        {
            printf("Numero nao encontrado");
        }
    }

    printf("\n");

    printf("Quantidade de 1's no primeiro vetor ->: %d\n", contador1);
    printf("Quantidade de 2's no segundo vetor -> : %d\n", contador2);
    printf("Quantidade de 3's no terceiro vetor -> : %d\n", contador3);

    printf("\nAmbas as threads 1, 2 e 3 ja finalizaram e agora eu finalizarei...\n");
    exit(0);
}

// Imprime a mensagem pedida 1000 vezes
void *funcaoImprimeMensagem(void *ptr)
{
    char *mensagem;
    mensagem = (char *)ptr;
    for (int i = 0; i < 1000; i++)
    {
        printf("%s \n", mensagem);
    }
}

// Função que preenche os vetores e converte o valor char definido na mensagem para inteiro, e preenche o vetor de forma correta

void *funcaopreenchevetor(void *ptr)
{
    char *mensagem = (char *)ptr;
    int inteiro = atoi(mensagem); // Converte a string em um número inteiro
    printf("%d\n", inteiro);

    for (int i = (inteiro - 1) * 1000; i < inteiro * 1000; i++)
    {
        vet[i] = inteiro;
    }
}
