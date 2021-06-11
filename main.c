#include "main.h"
#include "watek_komunikacyjny.h"
#include "watek_glowny.h"
#include "monitor.h"

#include "structs.h"
#include "queue.h"


/* wątki */
#include <pthread.h>

/* sem_init sem_destroy sem_post sem_wait */
//#include <semaphore.h>
/* flagi dla open */
//#include <fcntl.h>

state_t stan=InRun;
volatile char end = FALSE;

int size,rank, Z;

MPI_Datatype MPI_PAKIET_T;
pthread_t threadKom, threadMon;

pthread_mutex_t energy_mut = PTHREAD_MUTEX_INIT;



pthread_mutex_t stateMut = PTHREAD_MUTEX_INITIALIZER;

void check_thread_support(int provided)
{
    printf("THREAD SUPPORT: %d\n", provided);
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
        case MPI_THREAD_MULTIPLE: printf("Pełne wsparcie dla wątków\n");
	    break;
        default: printf("Nikt nic nie wie\n");
    }
}

/* srprawdza, czy są wątki, tworzy typ MPI_PAKIET_T
*/
void inicjuj(int *argc, char ***argv)
{
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
    MPI_Datatype typy[4] = {MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint     offsets[4]; 
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);
    offsets[2] = offsetof(packet_t, data);
    offsets[3] = offsetof(packet_t, which);


    MPI_Type_create_struct(nitems, blocklengths, offsets, typy, &MPI_PAKIET_T); // tworzy typ MPI z podanych danych 
    MPI_Type_commit(&MPI_PAKIET_T);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // zwraca ranking procesu
    MPI_Comm_size(MPI_COMM_WORLD, &size); // zwraca ilosc procesow w grupie
    srand(rank);

    pthread_create( &threadKom, NULL, startKomWatek , 0);
    if (rank==0) {
	pthread_create( &threadMon, NULL, startMonitor, 0);
    }
    debug("jestem");
}

/* usunięcie zamkków, czeka, aż zakończy się drugi wątek, zwalnia przydzielony typ MPI_PAKIET_T
   wywoływane w funkcji main przed końcem
*/
void finalizuj()
{
    pthread_mutex_destroy( &stateMut);
    /* Czekamy, aż wątek potomny się zakończy */
    println("czekam na wątek \"komunikacyjny\"\n" );
    pthread_join(threadKom,NULL);
    if (rank==0) pthread_join(threadMon,NULL);
    MPI_Type_free(&MPI_PAKIET_T); // zwalnia pakiet danych
    MPI_Finalize(); //zakonczenie procesu przed exitem
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
    pthread_mutex_lock(&callowMut);
    lclock = (lclock+1 > newClock)? (lclock+1):newClock;
    pthread_mutex_unlock(&callowMut);
    return lclock;
}

int increaseClock(int unit ){
    pthread_mutex_lock(&callowMut);
    lclock+=newClock;
    pthread_mutex_unlock(&callowMut);
    return lclock;
}

void changeState( state_t newState )
{
    increaseClock(1);
    pthread_mutex_lock( &stateMut );
    stan = newState;
	pthread_mutex_unlock( &stateMut );
    
    
   
}

int main(int argc, char **argv)
{
    
   
    
    
    /* Tworzenie wątków, inicjalizacja itp */
    inicjuj(&argc,&argv);
    
    
     // tworzy wątek komunikacyjny w "watek_komunikacyjny.c"
    
    E = 10;
    
    
    
    mainLoop();          // w pliku "watek_glowny.c"

    finalizuj();
    return 0;
}

