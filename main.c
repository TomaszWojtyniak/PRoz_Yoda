#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int clockLamport = 0;
int stop = 0;


pthread_mutex_t	mutexClock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutexCond = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct data
{
    int rank; // my own rank
    int size; // how many skiers
    int * tab_ack; // did I receive ack from a skier with that id? 1/0
    queue_el *head; // pointer to a head of a queue
};

int main(int argc, char **argv)
{
	
	srand(time(NULL)); 
    int rank,size;


    int provided = 0;
    MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE,  &provided);
    if (provided < MPI_THREAD_MULTIPLE)
    {
        printf("ERROR: The MPI library does not have full thread support\n");

    }
    else
    {
        printf("Full support for multiple threads!\n");
    }


    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &size);
    struct data dane;
    dane.rank=rank;
    dane.size=size;
	dane.head=NULL;
	srand(rank);
    dane.tab_ack = malloc(dane.size*sizeof(int));
    for (int i = 0; i < dane.size; i++)
    {
        dane.tab_ack[i] = 0;
    }
    dane.tab_ack[dane.rank] = 1; // set ack from yourself to 1

	printf("Wątek %d zainicjował zmienne i rozpocząl działnie.\n", dane.rank);
    pthread_t watek1,watek2;
    // pthread_create(&watek1,NULL,receiveAndSendAck,&dane);
    // pthread_create(&watek2,NULL,mainSkiThread,&dane);
    pthread_join(watek1,NULL);
    pthread_join(watek2,NULL);
    pthread_mutex_destroy(&mutexClock);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutexCond);
    free(dane.tab_ack);
    MPI_Finalize();
	printf("Koniec programu");
}
