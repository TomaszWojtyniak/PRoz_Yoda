
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "watek_komunikacyjny.h"
#include "watek_glowny.h"
#include "monitor.h"
#include "structs.h"
/* wątki */
>>>>>>> parowanie
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int clockLamport = 0;
int stop = 0;

<<<<<<< HEAD

state_t stan=REST;
volatile char end = FALSE;
int size,rank, zegar, pairCounter, para;
MPI_Datatype MPI_PAKIET_T;
pthread_t threadKom;
struct_t structQueue;

pthread_mutex_t stateMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t zegarMut = PTHREAD_MUTEX_INITIALIZER;

>>>>>>> parowanie


int main(int argc, char **argv)
{
	
	srand(time(NULL)); 
    int rank,size;
=======
    int provided;
    MPI_Init_thread(argc, argv,MPI_THREAD_MULTIPLE, &provided);
    check_thread_support(provided);


    /* Stworzenie typu */
    /* Poniższe (aż do MPI_Type_commit) potrzebne tylko, jeżeli
       brzydzimy się czymś w rodzaju MPI_Send(&typ, sizeof(pakiet_t), MPI_BYTE....
    */
    /* sklejone z stackoverflow */
    const int nitems=4; /* bo packet_t ma trzy pola */
    int       blocklengths[4] = {1,1,1,1};
    MPI_Datatype typy[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint     offsets[4]; 
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);
    offsets[2] = offsetof(packet_t, data);
    offsets[3] = offsetof(packet_t, master);

    MPI_Type_create_struct(nitems, blocklengths, offsets, typy, &MPI_PAKIET_T);
    MPI_Type_commit(&MPI_PAKIET_T);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(rank);

    initQueue(&structQueue, size);

    pthread_create( &threadKom, NULL, startKomWatek , 0);
    debug("Jestem zainicjowany");

}

/* usunięcie zamkków, czeka, aż zakończy się drugi wątek, zwalnia przydzielony typ MPI_PAKIET_T
   wywoływane w funkcji main przed końcem
*/
void finalizuj()
{
    pthread_mutex_destroy( &stateMut);
    pthread_mutex_destroy( &zegarMut);
    /* Czekamy, aż wątek potomny się zakończy */
    println("czekam na wątek \"komunikacyjny\"\n" );
    pthread_join(threadKom,NULL);
    MPI_Type_free(&MPI_PAKIET_T);
    MPI_Finalize();
}
>>>>>>> parowanie


    int provided = 0;
    MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE,  &provided);
    if (provided < MPI_THREAD_MULTIPLE)
    {
        printf("ERROR: The MPI library does not have full thread support\n");

void changeState( state_t newState )
{
    // pthread_mutex_lock( &stateMut );
    // if (stan==RELEASE) { 
	// pthread_mutex_unlock( &stateMut );
    //     return;
    // }
    stan = newState;
    // pthread_mutex_unlock( &stateMut );
}
>>>>>>> parowanie


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
