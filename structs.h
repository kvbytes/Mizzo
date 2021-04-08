#ifndef STRUCTS
#define STRUCTS

#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include <semaphore.h>
#include <pthread.h>

// macros
#define MAXFROGS 3
#define MAXBELT 10
#define MAXPRODUCED 100

#define FROG 0
#define ESCARGOT 1

/*  MIZZO struct
 *
 *  This struct contains variables that will
 *  function similarly to a queue, and will have
 *  pointers for every semaphore.
*/
typedef struct MIZZO
{
    char ** candyInBelt;
    char ** name;
    int * head;
    int * tail;
    int N; // flag value
    int produced; // count of candies produced
    int consumed[2]; // separate count of frogs and escargots

    sem_t * beltMutex;
    sem_t * maxFrogs;
    sem_t * maxBelt;
    sem_t * inBelt;
    sem_t * maxProduced;
    sem_t * maxConsumed;
} MIZZO;

MIZZO ethelThread;
MIZZO lucyThread;
MIZZO frogThread;
MIZZO escargotThread;

char * belt[10];

// names of every thread, used primarily in output
char * names[4] = {"Ethel", "Lucy", "crunchy frog bite", "everlasting escargot sucker"};
pthread_t threads[4];

sem_t beltMutex;
sem_t maxFrogs;
sem_t maxBelt;
sem_t inBelt;
sem_t maxProduced;
sem_t maxConsumed;


#endif // STRUCTS

