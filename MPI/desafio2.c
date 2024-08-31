/*
  Desafio 2: somar os elementos de um vetor de inteiros da seguinte forma: 
              o processo 0 envia partes do vetor original para os demais 
              processos, cada processo calcula a soma da sua parte e 
              envia o resultado para o processo 0, que calcula a soma total
              e mostra na tela
*/

#include <stdio.h>
#include <mpi.h>
#include "../../../../usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h"
#include <string.h>

// Guilherme Frare Clemente RA:124349

#define TAM_STRING 100
#define TAM_VETOR 100

// Desafio 2
int main(int argc, char *argv[])
{ int id, np,i, tag=1, vetor[100], n = 100;
  int soma = 0;
  int somaTotal = 0;
  int inicio, fim, tamanho;

  MPI_Status st;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Comm_size(MPI_COMM_WORLD,&np);

  if(id == 0) {
    // Inicializa o vetor com valores de 1 a 100
    for(int i = 0; i < n; i++) {
        vetor[i] = i + 1;
    }

    tamanho = n / np;
    
    for(int i = 1; i < np; i++) {
        inicio = i * tamanho;
        fim = (i + 1) * tamanho;
        // Envia apenas uma parte do vetor para o outro processo
        MPI_Send(&vetor[inicio], fim - inicio, MPI_INT, i, tag, MPI_COMM_WORLD);
    }

    for(int i = 0; i < tamanho; i++){
        soma += vetor[i];
    }

    for(int i = 1; i < np; i++){
        // Soma parcialmente os valores dos vetores armazenados pelo processo 0
        int somaParcial;
        MPI_Recv(&somaParcial, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &st);
        somaTotal += somaParcial;
    }

    somaTotal += soma;
    printf("Soma total: %d\n", somaTotal);

  } else {
    tamanho = n / np;
    int subvetor[tamanho];

    // Recebe o subvetor enviado para continuar o processo de soma dos valores do vetor
    MPI_Recv(subvetor, tamanho, MPI_INT, 0, tag, MPI_COMM_WORLD, &st);

    for(int i = 0; i < tamanho; i++){
        soma += subvetor[i];
    }
    
    // Envia a soma parcial 
    MPI_Send(&soma, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}
