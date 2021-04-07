#include "structs.h"

void * producer(void * data) {
    MIZZO *m = (MIZZO *) data;
    m->produced = 0;
    while (1) {
        if (sem_trywait(m->maxProduced) == -1) { // check if we've reached 100
            pthread_exit(NULL);
        }
        if (*m->name == "FrogBite") { // check if there's 3 frogs
            sem_wait(m->maxFrogs);
        }
        sem_wait(m->maxBelt);
        sem_wait(m->beltMutex);
        m->candyInBelt[*m->tail] = *m->name;
        m->produced++;

        *m->tail = (*m->tail + 1) % 10;

        printf("produced %s\n", *m->name);

        sem_post(m->beltMutex);
        sem_post(m->inBelt);

        usleep(m->N * 1000); // convert to micro
    }
}

void * consumer(void * data) {
    MIZZO *m = (MIZZO *) data;
    m->consumed[FROG] = 0;
    m->consumed[ESCARGOT] = 0;
    while (1) {
        //Exit if consume limit has been hit
        if (sem_trywait(m->maxConsumed) == -1) {
            pthread_exit(NULL);
        }
        sem_wait(m->inBelt);
        sem_wait(m->beltMutex);
        if (m->candyInBelt[*m->head] == "FrogBite") {
            sem_post(m->maxFrogs);
            m->consumed[FROG]++;
        } else {
            m->consumed[ESCARGOT]++;
        }

        printf("%s consumed %s\n", *m->name, m->candyInBelt[*m->head]);

        m->candyInBelt[*m->head] = "";
        *m->head = (*m->head + 1) % 10;
        sem_post(m->beltMutex);
        sem_post(m->maxBelt);

        usleep(m->N * 1000); // convert to micro
    }
}


int main(int argc, char *argv[]) {

    int EN = 0;
    int LN = 0;
    int fN = 0;
    int eN = 0;

    int head = 0;
    int tail = 0;

    int Option;
    //int flagValues[4] = {0, 0, 0, 0};
    while ((Option = getopt(argc, argv, "E:L:f:e:")) != -1) {
        switch (Option) {
            case 'E' :
                EN = atoi(optarg);
                break;
            case 'L' :
                LN = atoi(optarg);
                break;
            case 'f' :
                fN = atoi(optarg);
                break;
            case 'e':
                eN = atoi(optarg);
                break;
        }
    }

    // maximum of 3 frog bites in the belt
    sem_init(&maxFrogs, 0, 3);
    // maximum of 10 total candies in the belt
    sem_init(&maxBelt, 0, 10);
    sem_init(&inBelt, 0, 0);
    sem_init(&beltMutex, 0, 1);
    // semaphores for the producer and consumer count
    sem_init(&maxProduced, 0, 100);
    sem_init(&maxConsumed, 0, 100);

    // flags
    ethelThread.N = EN;
    lucyThread.N = LN;
    frogThread.N = fN;
    escargotThread.N = eN;

    // initialize mizzo arguments for all threads
    ethelThread.maxFrogs = lucyThread.maxFrogs = frogThread.maxFrogs = escargotThread.maxFrogs = &maxFrogs;
    ethelThread.maxBelt = lucyThread.maxBelt = frogThread.maxBelt = escargotThread.maxBelt = &maxBelt;
    ethelThread.beltMutex = lucyThread.beltMutex = frogThread.beltMutex = escargotThread.beltMutex = &beltMutex;
    ethelThread.inBelt = lucyThread.inBelt = frogThread.inBelt = escargotThread.inBelt = &inBelt;
    ethelThread.maxProduced = lucyThread.maxProduced = frogThread.maxProduced = escargotThread.maxProduced = &maxProduced;
    ethelThread.maxConsumed = lucyThread.maxConsumed = frogThread.maxConsumed = escargotThread.maxConsumed = &maxConsumed;
    ethelThread.candyInBelt = lucyThread.candyInBelt = frogThread.candyInBelt = escargotThread.candyInBelt = belt;
    ethelThread.head = lucyThread.head = frogThread.head = escargotThread.head = &head;
    ethelThread.tail = lucyThread.tail = frogThread.tail = escargotThread.tail = &tail;

    ethelThread.name = &names[0];
    lucyThread.name = &names[1];
    frogThread.name = &names[2];
    escargotThread.name = &names[3];


    // create threads for consumers and producers
    pthread_create(&threads[0], NULL, &consumer, (void *) &ethelThread);
    pthread_create(&threads[1], NULL, &consumer, (void *) &lucyThread);
    pthread_create(&threads[2], NULL, &producer, (void *) &frogThread);
    pthread_create(&threads[3], NULL, &producer, (void *) &escargotThread);


    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);


    sem_destroy(&maxFrogs);
    sem_destroy(&maxBelt);
    sem_destroy(&inBelt);
    sem_destroy(&beltMutex);
    sem_destroy(&maxProduced);
    sem_destroy(&maxConsumed);

    // print totals
    printf("\n\n");
    printf("frogs produced: %d\n", frogThread.produced);
    printf("escargots produced: %d\n", escargotThread.produced);
    printf("Lucy consumed %d frogs and %d escargots = %d\n", lucyThread.consumed[FROG], lucyThread.consumed[ESCARGOT], lucyThread.consumed[FROG] + lucyThread.consumed[ESCARGOT]);
    printf("Ethel consumed %d frogs and %d escargots = %d\n", ethelThread.consumed[FROG], ethelThread.consumed[ESCARGOT], ethelThread.consumed[FROG] + ethelThread.consumed[ESCARGOT]);



    pthread_exit(NULL);
    return 0;
}


