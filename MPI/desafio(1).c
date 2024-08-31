#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

// Guilherme Frare Clemente - RA:124349

#define maxNumero 10 /* valor limite dos elementos das matrizes */

/* o tipo matriz eh apenas um tipo ponteiro, que apontara
   para uma memoria a ser alocada com malloc */

typedef int *tipoMatriz;

/* faz a leitura das dimensoes das matrizes A e B, forcando que o
   numero de linhas de A seja multiplo do numero de processos */

tipoMatriz A, B, C; /* A*B=C */

int mA, nA, mB, nB, mC, nC;

struct { int mA, nA, nB;
} dimensoes;

void leDim(int np, int *nlA, int *ncA, int *nlB, int *ncB)
{   int tam;

  printf("\nLeitura das Dimensoes das Matrizes A e B:\n");

    do
    { printf("\nDigite o numero de linhas da matriz A");
      printf("\nDeve ser multiplo do numero de processos %d",np);
      printf("\n> ");
      scanf("%d", nlA);
    } while ((*nlA<1)||(((*nlA)%np)!=0));

    do
    { printf("\nDigite o numero de colunas da matriz A");
      printf("\nDeve ser > 0");
      printf("\n> ");
      scanf("%d", ncA);
    } while (*ncA<1);

    printf("\nAtencao!: O numero de colunas de A eh igual");
    printf("\n          ao numero de linhas de B!");
    *nlB=*ncA;

    do
    { printf("\nDigite o numero de colunas da matriz B");
      printf("\nDeve ser > 0");
      printf("\n> ");
      scanf("%d", ncB);
    } while (*ncB<1);

}

/*
   Observe que as matrizes s�o alocadas como uma sequencia de
   bytes contiguos, como se fosse um vetor. As linhas s�o
   colocadas/alocadas uma apos a outra sobre esta sequencia de
   bytes. Nao existe indexacao matricial de linha x coluna, apenas
   posicionamento linear. O programa converte as posicoes matriciais do
   tipo linha x coluna em posi��es lineares e tambem faz o contrario:
   converte as posi��es lineares em posicoes matriciais do tipo
   linha x coluna. Essas convers�es facilita muito a programacao,
   permitindo trabalhar com a logica matricial em vetores.

*/

int converteCoordenadasMV(int lin, int col, int nc)
{
  return (lin*nc+col);
}

void converteCoordenadasVM(int pos, int nc, int *lin, int *col)
{
  (*lin)= pos/nc;
  (*col)= pos%nc;
}

/* faz a geracao aleatoria das matrizes A e B, de acordo com
   as dimensoes fornecidas. Os elementos gerados sao limitados a
   um valor maximo (maxNumero).
   */

void geraMatrizes(tipoMatriz A, tipoMatriz B, int mA, int nA, int nB)
{   int i, sinal;

    srand(time(NULL));

    printf("\nGeracao Aleatoria da Matriz A\n");

    for (i=0; i<(mA*nA); i++)
    {
      A[i]=rand()%maxNumero; /* % retorna o resto da divisao inteira */
      sinal=(rand()%2)*2-1;  /* se rand%2==0, sinal=-1;
                                 se rand%2==1, sinal=+1 */

      A[i]=sinal*A[i];  /* imp�e o sinal no n�mero */
    }

    mB=nA;
    for (i=0; i<(mB*nB); i++)
    {
      B[i]=rand()%maxNumero; /* % retorna o resto da divisao inteira */
      sinal=(rand()%2)*2-1;  /* se rand%2==0, sinal=-1;
                                 se rand%2==1, sinal=+1 */

      B[i]=sinal*B[i];  /* imp�e o sinal no n�mero */
    }
}

/* mostra a matriz na tela do computador */

void mostraMatriz(char *id, tipoMatriz M, int m, int n)
{   int lin, col, k;

    printf("\nElementos da Matriz %s:\n", id);
    k=0;
    for (lin=0; lin<m; lin++)
    {
      for (col=0; col<n; col++)
      {
        printf("%6d",M[k]);
        k++;
      }
      printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{ int id, np, i, lin, col, k, proc, pos, tag=1, nlpp, prod;

  MPI_Status st;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&id); /* retotna o id do processo */
  MPI_Comm_size(MPI_COMM_WORLD,&np); /* retorna o numero de processos */

  if(id == 0)
  { /* processo 0 le a dimensao das matrizes A e B e envia
       para os demais processos o numero de linhas de A por processo,
       bem como a matriz B completa */

    leDim(np,&mA, &nA, &mB, &nB);

    mC = mA;
    nC = nB;
    nlpp = mA / np; /* numero de linhas de A por processo */

    dimensoes.mA = mA;
    dimensoes.nA = nA;
    dimensoes.nB = nB;

    A = malloc(mA * nA * sizeof(int));
    B = malloc(mB * nB * sizeof(int));

    geraMatrizes(A, B, mA, nA, nB);

    mostraMatriz("A", A, mA, nA);

    mostraMatriz("B", B, mB, nB);

    printf("\nProcesso 0 vai enviar dimensoes e matriz B ...\n");

    /* Envia dimensões e matriz B para todos os processos */
    MPI_Bcast(&dimensoes, sizeof(dimensoes), MPI_BYTE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, mB * nB, MPI_INT, 0, MPI_COMM_WORLD);

    printf("\nProcesso 0 vai enviar partes de A ...\n");

    /* Envia as linhas de A para cada processo */
    for (proc = 1; proc < np; proc++)
      MPI_Send(&(A[(proc * nlpp) * nA]), nlpp * nA, MPI_INT, proc, tag, MPI_COMM_WORLD);

    printf("\nProcesso 0 enviou dimensoes, B e partes de A ...\n", id);
  }
  else
  {
    /* Demais processos recebem as informacoes enviadas pelo processo 0 */

    MPI_Bcast(&dimensoes, sizeof(dimensoes), MPI_BYTE, 0, MPI_COMM_WORLD);
    mA = dimensoes.mA;
    nA = dimensoes.nA;
    mB = nA;
    nB = dimensoes.nB;

    mC = mA;
    nC = nB;
    nlpp = mA / np; /* numero de linhas de A por processo */

    B = malloc(mB * nB * sizeof(int));
    MPI_Bcast(B, mB * nB, MPI_INT, 0, MPI_COMM_WORLD);

    /* Para os processos, A contera apenas as linhas de cada um */
    A = malloc(nlpp * nA * sizeof(int));
    MPI_Recv(A, nlpp * nA, MPI_INT, 0, tag, MPI_COMM_WORLD, &st);
  }

  if (id == 0)
  {
    C = malloc(mC * nC * sizeof(int));

    /* Calcula a sua parte na multiplicacao matricial */
    for (lin = 0; lin < nlpp; lin++)
    {
      for (col = 0; col < nB; col++)
      {
        prod = 0;
        for (k = 0; k < nA; k++)
          prod = prod + (A[converteCoordenadasMV(lin, k, nA)] *
                         B[converteCoordenadasMV(k, col, nB)]);

        C[converteCoordenadasMV(lin, col, nC)] = prod;
      }
    }

    /* Recebe os resultados dos demais: recebe as linhas de C de cada processo */
    printf("\nProcesso 0 vai receber partes de C ...\n", id);
    for (proc = 1; proc < np; proc++)
      MPI_Recv(&(C[(proc * nlpp) * nC]), nlpp * nC, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &st);

    mostraMatriz("C", C, mC, nC);
  }
  else
  {
    /* Calcula a sua parte na multiplicacao matricial */
    C = malloc(nlpp * nC * sizeof(int));

    for (lin = 0; lin < nlpp; lin++)
    {
      for (col = 0; col < nB; col++)
      {
        prod = 0;
        for (k = 0; k < nA; k++)
          prod = prod + (A[converteCoordenadasMV(lin, k, nA)] *
                         B[converteCoordenadasMV(k, col, nB)]);

        C[converteCoordenadasMV(lin, col, nC)] = prod;
      }
    }

    /* Envia as linhas calculadas da matriz C para o processo 0 */
    MPI_Send(C, nlpp * nC, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}
