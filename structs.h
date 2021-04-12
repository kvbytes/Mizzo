/* Karla Barrios (Section 05 - 12:30pm)
 *       RedID: 823557131
 *       CSSC : cssc3502
 *  Jordan Duong (Section 03 - 11:00am)
 *       RedID: 822600994
 *       CSSC : cssc3419
 *
 *  Pair Programmer Affidavit: We the undersigned promise that we have in good faith
 *  attempted to follow the principles of pair programming. Although we were free to discuss
 *  ideas with others, the implementation is our own. We have shared a common workspace and
 *  taken turns at the keyboard for the majority of the work that we are submitting.
 *  Furthermore, any non programming portions of the assignment were done independently.
 *  We recognize that should this not be the case, we will be subject to penalties as outlined
 *  in the course syllabus.
 */

#ifndef STRUCTS
#define STRUCTS

#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include <semaphore.h>
#include <pthread.h>
#include <time.h>

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

double elapsed;
//clock_t start;
//clock_t current;
struct timespec start;
struct timespec current;

sem_t beltMutex;
sem_t maxFrogs;
sem_t maxBelt;
sem_t inBelt;
sem_t maxProduced;
sem_t maxConsumed;


#endif // STRUCTS
