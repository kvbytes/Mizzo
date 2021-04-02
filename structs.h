#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pthread.h>
#include <semaphore.h>

typedef enum PRODUCER
{
    frog,
    escargot
} PRODUCER;

typedef enum CONSUMER
{
    Ethel,
    Lucy
} CONSUMER;

typedef struct LIST
{
    PRODUCER type;
    struct LIST * next;
} LIST;

typedef struct MIZZO
{
    PRODUCER prod;
    CONSUMER cons;

    bool Ethel;
    bool Lucy;
    bool frog;
    bool escargot;

    int candyProduced;
    int candyConsumed;
    int candyMax;
    int candyInBelt;
    LIST * candyBeltList;

    // semaphores
    sem_t beltSem;
    sem_t typeSem;
} MIZZO;
