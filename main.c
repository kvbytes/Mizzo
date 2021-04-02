#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "structs.h"

int main(int argc, char ** argv)
{
    int Option;
    bool E = false;
    int EN = 0;
    bool L = false;
    int LN = 0;
    bool f = false;
    int fN = 0;
    bool e = false;
    int eN = 0;

    while ( (Option = getopt(argc, argv, "E:L:f:e:")) != -1)
    {
        switch (Option)
        {
            case 'E':
                E = true;
                EN = atoi(optarg);
                break;

            case 'L':
                L = true;
                LN = atoi(optarg);
                break;

            case 'f':
                f = true;
                fN = atoi(optarg);
                break;

            case 'e':
                e = true;
                eN = atoi(optarg);
                break;
        }
    }

    // initialize the struct and its variables
    MIZZO * m = malloc(sizeof(MIZZO));
    m->candyProduced = 0;
    m->candyConsumed = 0;
    m->candyMax = 100; // maximum of 100 total candies produced
    m->candyInBelt = 0; // maximum of 10 in belt
    m->candyBeltList = NULL;

    // command line delays
    m->Ethel = EN;
    m->Lucy = LN;
    m->frog = fN;
    m->escargot = eN;

    // initialize semaphores
    sem_init(&m->beltSem, 0, 1);
    sem_init(&m->typeSem, 0, 1);

    // initialize threads
    pthread_t frogThread;
    pthread_create(&frogThread, NULL, 0, m);
	pthread_t escargotThread;
	pthread_create(&escargotThread, NULL, 0, m);
	pthread_t EthelThread;
	pthread_create(&EthelThread, NULL, 0, m);
	pthread_t LucyThread;
	pthread_create(&LucyThread, NULL, 0, m);

	pthread_join(frogThread, NULL);
	pthread_join(escargotThread, NULL);
	pthread_join(EthelThread, NULL);
	pthread_join(LucyThread, NULL);




    return 0;
}
