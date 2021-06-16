#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "watek_komunikacyjny.h"
#include "watek_glowny.h"
#include "monitor.h"
#include "structs.h"
/* wątki */

#include <pthread.h>
#include <unistd.h>
#include <time.h>

int clockLamport = 0;
int stop = 0;



state_t stan=INIT;
volatile char end = FALSE;
int size,rank, zegar, pairCounter, para, which, E;





MPI_Datatype MPI_PAKIET_T;
pthread_t threadKom;
struct_t structQueue;

pthread_mutex_t stateMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clockLMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t energyMut = PTHREAD_MUTEX_INITIALIZER;

void check_thread_support(int provided)
{
    printf("THREAD SUPPORT: chcemy %d. Co otrzymamy?\n", provided);
    switch (provided) {
        case MPI_THREAD_SINGLE: 
            printf("Brak wsparcia dla wątków, kończę\n");
            /* Nie ma co, trzeba wychodzić */
	    fprintf(stderr, "Brak wystarczającego wsparcia dla wątków - wychodzę!\n");
	    MPI_Finalize();
	    exit(-1);
	    break;
        case MPI_THREAD_FUNNELED: 
            printf("tylko te wątki, ktore wykonaly mpi_init_thread mogą wykonać wołania do biblioteki mpi\n");
	    break;
        case MPI_THREAD_SERIALIZED: 
            /* Potrzebne zamki wokół wywołań biblioteki MPI */
            printf("tylko jeden watek naraz może wykonać wołania do biblioteki MPI\n");
	    break;
        case MPI_THREAD_MULTIPLE: printf("Pełne wsparcie dla wątków\n"); /* tego chcemy. Wszystkie inne powodują problemy */
	    break;
        default: printf("Nikt nic nie wie\n");
    }
}


int main(int argc, char **argv)
{
	
    E = 0;

    inicjuj(&argc,&argv); // tworzy wątek komunikacyjny w "watek_komunikacyjny.c"
    mainLoop();          // w pliku "watek_glowny.c"
    finalizuj();
    return 0;
    
    

}



void inicjuj(int *argc, char ***argv){
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
    initQueue(&waitQueue, size);

    pthread_create( &threadKom, NULL, startKomWatek , 0);
    debug("Jestem zainicjowany");
}

/* usunięcie zamkków, czeka, aż zakończy się drugi wątek, zwalnia przydzielony typ MPI_PAKIET_T
   wywoływane w funkcji main przed końcem
*/
void finalizuj()
{
    pthread_mutex_destroy( &stateMut);
    pthread_mutex_destroy( &clockLMut);
    /* Czekamy, aż wątek potomny się zakończy */
    println("czekam na wątek \"komunikacyjny\"\n" );
    pthread_join(threadKom,NULL);
    MPI_Type_free(&MPI_PAKIET_T);
    MPI_Finalize();
}



   

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

void changeE()
{
    pthread_mutex_lock( &stateMut );
    E += 1;
    
    for(int i=0; i< size; i++){
        if(i != rank){
            packet_t * pkt = malloc(sizeof(packet_t));
            pkt->data = pr;
            sendPacket( pkt, i, ZWIEKSZAM);
        }
    }
    pthread_mutex_unlock( &stateMut );
}



void sendPacket(packet_t *pkt, int destination, int tag)
{
    // int freepkt=0;
    // if (pkt==0) { pkt = malloc(sizeof(packet_t)); freepkt=1;}
    // pkt->src = rank;
    // MPI_Send( pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    // if (freepkt) free(pkt);



    
    pkt->src = rank;
    pkt->ts = increaseClock(1);
    MPI_Send(pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
}

int updateClock(int unit ){
    pthread_mutex_lock(&clockLMut);
    zegar = (zegar+1 > unit)? (zegar+1):unit;
    pthread_mutex_unlock(&clockLMut);
    return zegar;
}

int increaseClock(int unit ){
    pthread_mutex_lock(&clockLMut);
    zegar+=unit;
    pthread_mutex_unlock(&clockLMut);
    return zegar;
}

int checkEnergy(){
    pthread_mutex_lock(&clockLMut);
    int result;
    if( E == 0){
        result =  1;
    } else if(E == size / 3){
        result =  0;
    }
    pthread_mutex_unlock(&clockLMut);
    return result;
}
