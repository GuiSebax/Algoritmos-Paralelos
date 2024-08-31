/* Desafio 1: substituir o envio de um inteiro por uma string.
*/

#include <stdio.h>
#include <mpi.h>
#include "../../../../usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h"
#include <string.h>

// Guilherme Frare Clemente RA:124349

#define TAM_STRING 100

// Desafio 1
int main(int argc, char *argv[])
{ int id, np,i, tag=1;
  char valor_env[TAM_STRING], valor_rec[TAM_STRING];

    MPI_Status st;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    if(id==0)
    {
        strcpy(valor_env, "Mensagem do Processo 0");
        MPI_Send(valor_env,strlen(valor_env) + 1,MPI_CHAR,1,tag, MPI_COMM_WORLD);
        MPI_Recv(valor_rec, 100 ,MPI_CHAR,1,tag, MPI_COMM_WORLD,&st);

        printf("Processo %d enviou: %s e recebeu: %s\n", id, valor_env, valor_rec);
    }
    else if (id==1)
    {
        strcpy(valor_env, "Resposta do processo 1");
        MPI_Send(valor_env,strlen(valor_env) + 1,MPI_CHAR,0,tag, MPI_COMM_WORLD);
        MPI_Recv(valor_rec, 100, MPI_CHAR, 0, tag, MPI_COMM_WORLD,&st);
        printf("Processo %d enviou: %s e recebeu: %s\n", id, valor_env, valor_rec);
    }
    else {
        printf("Processo %d não enviou nem recebem mensagem\n", id);
    }

    MPI_Finalize();

    return 0;

}
