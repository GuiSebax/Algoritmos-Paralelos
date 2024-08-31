#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define maxBuffer 10000
#define maxNumero 10000
#define maxThread 300

// Guilherme Frare Clemente RA:124349

typedef struct {
    int buffer[maxBuffer]; // local de armazenamento circular
    int in,     // posicao de insercao
        out,    // posicao de retirada
        count,  // quantidade de elementos no buffer
        tam;    // tamanho util do buffer
    pthread_mutex_t mutex; // mutex para o buffer
} tBuffer;

tBuffer buffer;

int nProd, // quantidade de dados a serem fabricados (producoes)
    nCons, // quantidade de dados a serem consumidos (consumos),
           // que nesta implementacao eh igual ao numero de producoes
    ntp,   // numero de threads produtoras
    ntc;   // numero de threads consumidoras

pthread_mutex_t prodConsMutex; // mutex para os contadores nProd e nCons

// inicializa a estrutura do buffer
void criaBuffer(int tam)
{
    buffer.in = -1;
    buffer.out = -1;
    buffer.count = 0;
    buffer.tam = tam;
    pthread_mutex_init(&buffer.mutex, NULL);
}

// deposita o dado no buffer
int deposita(int dado)
{
    pthread_mutex_lock(&buffer.mutex);
    if (buffer.count != buffer.tam)
    {
        buffer.in = (buffer.in + 1) % buffer.tam;
        buffer.buffer[buffer.in] = dado;
        buffer.count++;
        pthread_mutex_unlock(&buffer.mutex);
        return 1;
    }
    else
    {
        pthread_mutex_unlock(&buffer.mutex);
        return 0;
    }
}

// retira o dado do buffer
int retira(int *dado)
{
    pthread_mutex_lock(&buffer.mutex);
    if (buffer.count != 0)
    {
        buffer.out = (buffer.out + 1) % buffer.tam;
        *dado = buffer.buffer[buffer.out];
        buffer.count--;
        pthread_mutex_unlock(&buffer.mutex);
        return 1;
    }
    else
    {
        pthread_mutex_unlock(&buffer.mutex);
        return 0;
    }
}

// lista os elementos do buffer
void mostraBuffer(void)
{
    int i, in, out;

    in = buffer.in;
    out = buffer.out;

    printf("\nSituacao do Buffer:");
    printf("\nin=%d", in);
    printf("\nout=%d", out);

    printf("\nElementos Restantes no Buffer: ");

    while (in != out)
    {
        printf("%d, ", buffer.buffer[out]);
        out = (out + 1) % buffer.tam;
    }
}

// simula trabalho da thread com tempo "aleatorio"
void trabalha(void)
{
    int i, j, n, m;
    double soma;

/*
  trecho de codigo que simula o consumo de tempo
  de forma parcialmente aleatoria. Teste retirar
  o codigo do comentario(ativar o codigo).

  srand(time(NULL));
  n = 1000+(rand()%1000);
  m = 5000+(rand()%5000);
  soma = rand();
  for (i=0; i<n; i++)
    {   soma=((soma+(i*2)/2))*(rand()/soma);
        for (j=0; j<m; j++)
            soma=soma-rand()+soma;
    }
*/
}

// codigo da thread produtora
void *produtor(void *ptr)
{
    int *id, dado, sinal;

    id = (int *)ptr;

    srand(time(NULL));

    while (1)
    {
        pthread_mutex_lock(&prodConsMutex);
        if (nProd <= 0)
        {
            pthread_mutex_unlock(&prodConsMutex);
            break;
        }
        nProd--;
        pthread_mutex_unlock(&prodConsMutex);

        trabalha();

        printf("\nProdutor %d: vai produzir um dado", *id);

        // produz um dado positivo ou negativo aleatoriamente
        dado = rand() % maxNumero;
        sinal = (rand() % 2) * 2 - 1;
        dado = sinal * dado;

        printf("\nProdutor %d: produziu o dado %d", *id, dado);

        if (deposita(dado))
        {
            printf("\nProdutor %d: depositou o dado %d", *id, dado);
        }
        else
        {
            printf("\nProdutor %d: Nao depositou!", *id);
        }

        printf("\nProdutor %d: numero de producoes = %d", *id, nProd);
    }

    return NULL;
}

// codigo da thread consumidora
void *consumidor(void *ptr)
{
    int *id, dado;

    id = (int *)ptr;

    while (1)
    {
        pthread_mutex_lock(&prodConsMutex);
        if (nCons <= 0)
        {
            pthread_mutex_unlock(&prodConsMutex);
            break;
        }
        nCons--;
        pthread_mutex_unlock(&prodConsMutex);

        trabalha();

        printf("\nConsumidor %d: vai consumir um dado", *id);

        if (retira(&dado))
        {
            printf("\nConsumidor %d: consumiu o dado %d", *id, dado);
        }
        else
        {
            printf("\nConsumidor %d: Nao consumiu!", *id);
        }

        printf("\nConsumidor %d: numero de consumos = %d", *id, nCons);
    }

    return NULL;
}

// codigo da thread primaria, principal
int main()
{
    pthread_t tProd[maxThread], tCons[maxThread];
    int iretProd[maxThread], iretCons[maxThread],
        tp[maxThread], tc[maxThread], resp,
        i, pindex, cindex, tam;

    pthread_mutex_init(&prodConsMutex, NULL);

    do
    {
        printf("\nProblema do Produtor-Consumidor Multithreaded\n");

        do
        {
            printf("\nQual o tamanho util do buffer? (max = %d) => ", maxBuffer);
            scanf("%d", &tam);
        } while ((tam < 1) || (tam > maxBuffer));

        criaBuffer(tam);

        do
        {
            printf("\nQuantas threads produtoras? (max=%d) => ", maxThread - 1);
            scanf("%d", &ntp);
        } while ((ntp < 1) || (ntp >= maxThread));

        for (i = 0; i < ntp; i++)
            tp[i] = i;

        do
        {
            printf("\nQuantas threads consumidoras? (max=%d) => ", maxThread - ntp);
            scanf("%d", &ntc);
        } while ((ntc < 1) || (ntp + ntc > maxThread));

        for (i = 0; i < ntc; i++)
            tc[i] = i;

        do
        {
            printf("\nQuantas producoes deseja? => ");
            scanf("%d", &nProd);
        } while (nProd < 1);

        nCons = nProd;

        /* Cria threads produtoras e conumidoras de forma intercalada
           para balancear as atividades de producao e consumo */

        pindex = 0;
        cindex = 0;
        do
        {
            if (pindex < ntp)
            {
                iretProd[pindex] = pthread_create(&tProd[pindex], NULL, produtor, (void *)&tp[pindex]);
                pindex++;
            }

            if (cindex < ntc)
            {
                iretCons[cindex] = pthread_create(&tCons[cindex], NULL, consumidor, (void *)&tc[cindex]);
                cindex++;
            }
        } while ((pindex < ntp) || (cindex < ntc));

        trabalha();

        // espera todas as threads acabarem
        for (pindex = 0; pindex < ntp; pindex++)
            pthread_join(tProd[pindex], NULL);
        for (cindex = 0; cindex < ntc; cindex++)
            pthread_join(tCons[cindex], NULL);

        mostraBuffer();

        printf("\nDeseja Nova Execucao, <1>Sim <2>Nao? => ");
        scanf("%d", &resp);

    } while (resp == 1);

    return 0;
}
