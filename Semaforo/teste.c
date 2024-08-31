#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define maxBuffer 10000
#define maxNumero 10000
#define maxThread 300


// Guilherme Frare Clemente RA:124349

typedef struct {
    int buffer[maxBuffer]; // local de armazenamento circular
    int in, out, count, tam; // posicoes e contadores
} tBuffer;

tBuffer buffer;

int nProd, nCons, ntp, ntc;

pthread_mutex_t inMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t outMutex = PTHREAD_MUTEX_INITIALIZER;

sem_t empty, full;

// inicializa a estrutura do buffer
void criaBuffer(int tam) {
    buffer.in = 0;
    buffer.out = 0;
    buffer.count = 0;
    buffer.tam = tam;
}

// deposita o dado no buffer
void deposita(int dado) {
    sem_wait(&empty);
    pthread_mutex_lock(&inMutex);

    buffer.buffer[buffer.in] = dado;
    buffer.in = (buffer.in + 1) % buffer.tam;
    buffer.count++;

    pthread_mutex_unlock(&inMutex);
    sem_post(&full);
}

// retira o dado do buffer
void retira(int *dado) {
    sem_wait(&full);
    pthread_mutex_lock(&outMutex);

    *dado = buffer.buffer[buffer.out];
    buffer.out = (buffer.out + 1) % buffer.tam;
    buffer.count--;

    pthread_mutex_unlock(&outMutex);
    sem_post(&empty);
}

// lista os elementos do buffer
void mostraBuffer(void) {
    int in, out;

    in = buffer.in;
    out = buffer.out;

    printf("\nSituacao do Buffer:");
    printf("\nin=%d", in);
    printf("\nout=%d", out);

    printf("\nElementos Restantes no Buffer: ");

    while (out != in) {
        printf("%d, ", buffer.buffer[out]);
        out = (out + 1) % buffer.tam;
    }
}

// simula trabalho da thread com tempo "aleatorio"
void trabalha(void) {
    int i, j, n, m;
    double soma;

    // srand(time(NULL));
    // n = 1000 + (rand() % 1000);
    // m = 5000 + (rand() % 5000);
    // soma = rand();
    // for (i = 0; i < n; i++) {
    //     soma = ((soma + (i * 2) / 2)) * (rand() / soma);
    //     for (j = 0; j < m; j++) {
    //         soma = soma - rand() + soma;
    //     }
    // }
}

// codigo da thread produtora
void *produtor(void *ptr) {
    int *id = (int *)ptr;
    int dado, sinal;

    srand(time(NULL));

    while (1) {
        trabalha();

        pthread_mutex_lock(&inMutex);
        if (nProd <= 0) {
            pthread_mutex_unlock(&inMutex);
            break;
        }
        nProd--;
        pthread_mutex_unlock(&inMutex);

        printf("\nProdutor %d: vai produzir um dado", *id);

        dado = rand() % maxNumero;
        sinal = (rand() % 2) * 2 - 1;
        dado = sinal * dado;

        printf("\nProdutor %d: produziu o dado %d", *id, dado);

        deposita(dado);

        printf("\nProdutor %d: depositou o dado %d", *id, dado);
    }
    return NULL;
}

// codigo da thread consumidora
void *consumidor(void *ptr) {
    int *id = (int *)ptr;
    int dado;

    while (1) {
        trabalha();

        pthread_mutex_lock(&outMutex);
        if (nCons <= 0) {
            pthread_mutex_unlock(&outMutex);
            break;
        }
        nCons--;
        pthread_mutex_unlock(&outMutex);

        printf("\nConsumidor %d: vai consumir um dado", *id);

        retira(&dado);

        printf("\nConsumidor %d: consumiu o dado %d", *id, dado);
    }
    return NULL;
}

// codigo da thread primaria, principal
int main() {
    pthread_t tProd[maxThread], tCons[maxThread];
    int iretProd[maxThread], iretCons[maxThread], tp[maxThread], tc[maxThread], resp, i, pindex, cindex, tam;

    do {
        printf("\nProblema do Produtor-Consumidor Multithreaded\n");

        do {
            printf("\nQual o tamanho util do buffer? (max = %d) => ", maxBuffer);
            scanf("%d", &(tam));
        } while ((tam < 1) || (tam > maxBuffer));

        criaBuffer(tam);

        sem_init(&empty, 0, tam);
        sem_init(&full, 0, 0);

        do {
            printf("\nQuantas threads produtoras? (max=%d) => ", maxThread - 1);
            scanf("%d", &ntp);
        } while ((ntp < 1) || (ntp >= maxThread));

        for (i = 0; i < ntp; i++)
            tp[i] = i;

        do {
            printf("\nQuantas threads consumidoras? (max=%d) => ", maxThread - ntp);
            scanf("%d", &ntc);
        } while ((ntc < 1) || (ntp + ntc > maxThread));

        for (i = 0; i < ntc; i++)
            tc[i] = i;

        do {
            printf("\nQuantas producoes deseja? => ");
            scanf("%d", &nProd);
        } while (nProd < 1);

        nCons = nProd;

        pindex = 0;
        cindex = 0;
        do {
            if (pindex < ntp) {
                iretProd[pindex] = pthread_create(&tProd[pindex], NULL, produtor, (void *)&tp[pindex]);
                pindex++;
            }

            if (cindex < ntc) {
                iretCons[cindex] = pthread_create(&tCons[cindex], NULL, consumidor, (void *)&tc[cindex]);
                cindex++;
            }
        } while ((pindex < ntp) || (cindex < ntc));

        trabalha();

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
