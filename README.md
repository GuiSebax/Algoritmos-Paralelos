# Algoritmos-Paralelos

Dentro deste repositório está presente todos os algoritmo trabalhados na matéria de Programação Concorrente no curso de Ciências da computação - UEM

## Paralelização com Threads

Nesta aba possui a paralelização de algoritmos utilizando Threads, juntamente com MUTEX e Semáforos, então é possível vê-los na maioria dos algoritmos com Threads.

Para compilar utilize o seguinte comando

```
$cc -lpthread <arquivo>.c -o <executavel>
```

## Paralelização com MPI

Nesta aba possui a paralelização de algoritmos utilizando MPI, ou seja, dentro dos algoritmos possuem todo o processo de envio e recebimento de partes do mesmo algoritmo para cada processo executar individualmente as partes, gerando a paralelização.

Para compilar utilize o seguinte comando

```
$mpicc <arquivo>.c -o <executavel>

$mpirun --oversubscribe -np <numero de processos> <executavel>
```

O comando `--oversubscribe` é utilizado para permitir executar mais processos do que o número de processos de cores/processadores.
