/*

Exemplo 02: Testes com a identificacao de processos no OPENMPI.

Salve este arquivo com o nome exemplo02.c

Compile-o em um terminal pela linha de comando, da seguinte forma:

>mpicc exemplo02.c -o exemplo02 <enter>

Depois execute-o pela linha de comando, da seguinte forma:

>mpirun -np 5 ./exemplo02 bla ble bli blo blu <enter>

Atencao: talvez seja necessario utilizar a opcao --oversubscribe na
linha de comando do mpirun para permitir executar mais processos doque
o numero de cores/processadores.

Analise o codigo e a execucao. Teste diferentes numeros de processos.

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "../../../../usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h"

// Guilherme Frare Clemente RA:124349

#define VECTOR_SIZE 1000

int main(int argc, char *argv[]){
    int id, np, i;
    int vetor[VECTOR_SIZE];
    int start, end, local_size;
    double local_sum = 0, local_avg;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    for(int i = 0; i < VECTOR_SIZE; i++){
        vetor[i] = i + 1;
    }

    local_size = VECTOR_SIZE / np;
    start = id * local_size;
    if(id == np - 1){
        end = VECTOR_SIZE;
    } else {
        end = start + local_size;
    }

    for(int i = start; i < end; i++){
        local_sum += vetor[i];
    }

    local_avg = local_sum / local_size;

    printf("Processo %d:\n", id);
    printf("Intervalo: [%d, %d)\n", start, end);
    printf("Elementos: ");

    for(int i = start; i < end; i++){
        printf("%d ", vetor[i]);
    }

    printf("\nMédia local: %f\n\n", local_avg);

    MPI_Finalize();
    return 0;
}


/* Desafio: Faca um programa que declare e inicialize estaticamente um
vetor de 1000 elementos e divida esse vetor equilibradamente entre os
processos, de forma que cada processo ira calcular a media aritmetica
de sua parte. Cada processo deve imprimir na tela a sua identificacao
(id ou rank), o seu intervalo dos dados, a sua parte do vetor e a media
final */
