/*
 *  Producer-Consumer Problem Using POSIX Threads and Semaphores
 *  CS 570 - Operating Systems
 *  Spring 2021
*/

#include "structs.h"

/*  Producer
 *
 *  Takes in a "MIZZO" data structure corresponding to a producer
 *  thread. Produces the candy and pushes it to the tail of a queue.
 *  data: "MIZZO" data structure with the belt contents and semaphore
 *      pointers
*/
void * producer(void * data) {
    MIZZO * m = (MIZZO *) data;

    // initialize the total produced
    m->produced = 0;
    while (1) {
        // check if we've reached 100 produced
        if (sem_trywait(m->maxProduced) == -1) pthread_exit(NULL);
        // check if there's 3 frogs
        if (*m->name == "crunchy frog bite") sem_wait(m->maxFrogs);
        sem_wait(m->maxBelt);
        sem_wait(m->beltMutex);
        // insert a candy at the end of the queue and increment production total
        m->candyInBelt[*m->tail] = *m->name;
        m->produced++;
        *m->tail = (*m->tail + 1) % 10;

        // get the elapsed time
        clock_gettime(CLOCK_REALTIME, &current);
        long seconds = current.tv_sec - start.tv_sec;
        long nanoseconds = current.tv_nsec - start.tv_nsec;
        elapsed = seconds + nanoseconds*1e-9;

        int frogs = frogThread.produced - (ethelThread.consumed[FROG] + lucyThread.consumed[FROG]);
        int escargots = escargotThread.produced - (ethelThread.consumed[ESCARGOT] + lucyThread.consumed[ESCARGOT]);

        // print the belt and produced candy
        printf("Belt: %d CFB + %d EES", frogs, escargots);
        printf(" = %d. ", frogs + escargots);
        printf("Added %s. ", *m->name);
        printf("Produced: %d CFB + %d EES", frogThread.produced, escargotThread.produced);
        printf(" = %d ", frogThread.produced + escargotThread.produced);
        printf("in %.3f s.\n", elapsed);

        sem_post(m->beltMutex);
        sem_post(m->inBelt);

        usleep(m->N * 1000); // convert to micro
    }
}

/*  Consumer
 *
 *  Takes in a "MIZZO" data structure corresponding to a consumer
 *  thread. Lucy/Ethel consume the candy at the head of a queue.
 *  data: "MIZZO" data structure with the belt contents and semaphore
 *      pointers
*/

void * consumer(void * data) {
    MIZZO * m = (MIZZO *) data;
    m->consumed[FROG] = 0;
    m->consumed[ESCARGOT] = 0;
    while (1) {
        // check if we've reached 100 consumed
        if (sem_trywait(m->maxConsumed) == -1) pthread_exit(NULL);
        sem_wait(m->inBelt);
        sem_wait(m->beltMutex);
        if (m->candyInBelt[*m->head] == "crunchy frog bite") {
            sem_post(m->maxFrogs);
            m->consumed[FROG]++;
        } else m->consumed[ESCARGOT]++;

        // get the elapsed time
        clock_gettime(CLOCK_REALTIME, &current);
        long seconds = current.tv_sec - start.tv_sec;
        long nanoseconds = current.tv_nsec - start.tv_nsec;
        elapsed = seconds + nanoseconds*1e-9;

        int frogs = frogThread.produced - (ethelThread.consumed[FROG] + lucyThread.consumed[FROG]);
        int escargots = escargotThread.produced - (ethelThread.consumed[ESCARGOT] + lucyThread.consumed[ESCARGOT]);

        // print the belt and consumed candy
        printf("Belt: %d CFB + %d EES", frogs, escargots);
        printf(" = %d. ", frogs + escargots);
        printf("%s consumed %s. ", *m->name, m->candyInBelt[*m->head]);
        printf("%s totals: %d CFB + %d EES", *m->name, lucyThread.consumed[FROG], lucyThread.consumed[ESCARGOT]);
        printf(" consumed in %.3f s.\n", elapsed);

        m->candyInBelt[*m->head] = "";
        *m->head = (*m->head + 1) % 10;
        sem_post(m->beltMutex);
        sem_post(m->maxBelt);

        usleep(m->N * 1000); // convert to micro
    }
}


int main(int argc, char *argv[]) {

    // default flag values
    int EN = 0;
    int LN = 0;
    int fN = 0;
    int eN = 0;

    int head = 0;
    int tail = 0;

    // start the timer
    elapsed = 0;
    clock_gettime(CLOCK_REALTIME, &start);

    int Option;
    while ((Option = getopt(argc, argv, "E:L:f:e:")) != -1) {
        switch (Option) {
            case 'E':
                EN = atoi(optarg);
                break;
            case 'L':
                LN = atoi(optarg);
                break;
            case 'f':
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

    // initialize MIZZO data structure arguments for all threads
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
    pthread_create(&threads[0], NULL, consumer, (void *) &ethelThread);
    pthread_create(&threads[1], NULL, consumer, (void *) &lucyThread);
    pthread_create(&threads[2], NULL, producer, (void *) &frogThread);
    pthread_create(&threads[3], NULL, producer, (void *) &escargotThread);


    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);

    // destroy all semaphores
    sem_destroy(&maxFrogs);
    sem_destroy(&maxBelt);
    sem_destroy(&inBelt);
    sem_destroy(&beltMutex);
    sem_destroy(&maxProduced);
    sem_destroy(&maxConsumed);

    // print the summary report
    printf("\nPRODUCTION REPORT\n----------------------------------------\n");
    printf("crunchy frog bite producer generated %d candies\n", frogThread.produced);
    printf("everlasting escargot sucker producer generated %d candies\n", escargotThread.produced);
    printf("Lucy consumed %d CFB + %d EES = %d\n", lucyThread.consumed[FROG], lucyThread.consumed[ESCARGOT], lucyThread.consumed[FROG] + lucyThread.consumed[ESCARGOT]);
    printf("Ethel consumed %d CFB + %d EES = %d\n", ethelThread.consumed[FROG], ethelThread.consumed[ESCARGOT], ethelThread.consumed[FROG] + ethelThread.consumed[ESCARGOT]);

    // get the elapsed time
    clock_gettime(CLOCK_REALTIME, &current);
    long seconds = current.tv_sec - start.tv_sec;
    long nanoseconds = current.tv_nsec - start.tv_nsec;
    elapsed = seconds + nanoseconds*1e-9;
    printf("Elapsed time %.3f s\n", elapsed);

    pthread_exit(NULL);
    return 0;
}
