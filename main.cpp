#include "main.h"
#include "watek_komunikacyjny_Z.h"
#include "watek_komunikacyjny_XY.h"
#include "watek_glowny_Z.h"
#include "watek_glowny_XY.h"
/* wątki */

#include <pthread.h>
#include <time.h>

state_t stan;
volatile char end = FALSE;
int size; //ile procesow
int rank; //ID processu
int zegar; //Lamport
int pairCounter;
int para;
int which; //jaka klasa
int E; //ilosc energii
int isFilled;




MPI_Datatype MPI_PAKIET_T;
pthread_t threadKom;
Queue waitQueue;
Queue structQueue;

pthread_mutex_t stateMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clockLMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t energyMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ackMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waitQueueMut = PTHREAD_MUTEX_INITIALIZER;


std::map<int, bool> acksSent; //rank : true = ack sent, rank : false = ack not sent

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
	

    inicjuj(&argc,&argv); 
    if(which == 0){
        debug("ENERGIA %d", E);
        mainLoop_Z();

    } 
    // else {
    //     mainLoop_XY();
    // }   
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
    offsets[0] = offsetof(packet_t, zegar);
    offsets[1] = offsetof(packet_t, src);
    offsets[2] = offsetof(packet_t, isFilled);
    offsets[3] = offsetof(packet_t, E);
    

    

    MPI_Type_create_struct(nitems, blocklengths, offsets, typy, &MPI_PAKIET_T);
    MPI_Type_commit(&MPI_PAKIET_T);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(rank);

    zegar = 0;
    E = 0;
    which = rank %3;
    isFilled = TRUE;
    debug("Mam przydzielona role %d i zmieniam stan na REST",which);
    if(which == 0){
        changeState(WAIT_Z);
        pthread_create( &threadKom, NULL, startKomWatek_Z , 0);
    }

    // } else {
    //     changeState(REST_XY);
    //     pthread_create( &threadKom, NULL, startKomWatek_XY , 0);
    // }

    debug("Jestem zainicjowany");
}

/* usunięcie zamkków, czeka, aż zakończy się drugi wątek, zwalnia przydzielony typ MPI_PAKIET_T
   wywoływane w funkcji main przed końcem
*/
void finalizuj()
{
    pthread_mutex_destroy( &stateMut);
    pthread_mutex_destroy( &clockLMut);
    pthread_mutex_destroy( &energyMut);
    pthread_mutex_destroy( &ackMut);
    pthread_mutex_destroy( &waitQueueMut);

 

    /* Czekamy, aż wątek potomny się zakończy */
    println("czekam na wątek \"komunikacyjny\"\n" );
    pthread_join(threadKom,NULL);
    MPI_Type_free(&MPI_PAKIET_T);
    MPI_Finalize();
}



void changeState( state_t newState )
{
    increaseClock(1);
    pthread_mutex_lock( &stateMut );

    stan = newState;
    pthread_mutex_unlock( &stateMut );
}

void changeE(packet_t * pkt)
{
    debug("Jestem w change E")
    increaseClock(1);
    pthread_mutex_lock( &energyMut );
    
    E += 1;
    pkt->E = E;
    debug("Ile jest energi change E %d", pkt->E);
    sendPacketToAll(pkt, ZWIEKSZAM);

    pthread_mutex_unlock( &energyMut );
}


void decreaseE(packet_t * pkt){
    increaseClock(1);
    pthread_mutex_lock( &energyMut );
    E -= 1;
    pkt->E = E;
    sendPacketToAll(pkt, ZMNIEJSZAM);

    pthread_mutex_unlock( &energyMut );
}



void sendPacket(packet_t *pkt, int destination, int tag)
{
    increaseClock(1);
    pkt->src = rank;
    pkt->zegar = getClock();
    pkt->src = rank;
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

int getClock()
{
	int ts;
	pthread_mutex_lock(&clockLMut);
	ts = zegar;
	pthread_mutex_unlock(&clockLMut);
	return ts;
}

int checkEnergy(){
    pthread_mutex_lock(&energyMut); // było clockLMut czemu?
    int result;
    if( E == 0){ //pusta
        result =  1;
    } else if(E == size / 3){ //pelna
        result =  0;
    } else { // niepusta i niepelna
        result = 2;
    }
    pthread_mutex_unlock(&energyMut);
    return result;
}

void sendPacketToAll(packet_t* pkt, int tag)
{
	increaseClock(1); //LAMPORT CLOCK
	pkt->zegar = getClock();
	pkt->src = rank;
    pkt->E = E;
	for (int i = 0;i < size;  i++){
		if (i != rank){
			MPI_Send(pkt, 1, MPI_PAKIET_T, i, tag, MPI_COMM_WORLD);
        }
    }
}

void setClock(int newTime)
{
	pthread_mutex_lock(&clockLMut);
	if (newTime > zegar + 1)
		zegar = newTime;
	else
		zegar++;
	pthread_mutex_unlock(&clockLMut);
}

void recvPacket(packet_t  &pkt, MPI_Status &status)
{
	MPI_Recv(&pkt, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	setClock(pkt.zegar + 1);
}

void sendPacketToAllAndAddMeToSectionQueue(packet_t* pkt, int tag){
    increaseClock(1);
    pthread_mutex_lock(&clockLMut);
    pkt->zegar = zegar;
    pkt->src = rank;

    pthread_mutex_lock(&waitQueueMut);
    waitQueue.insert(rank, zegar,waitQueue.getFirst());
    pthread_mutex_unlock(&waitQueueMut);

    for (int i =0;i<size;i++){
        if(i != rank){
            MPI_Send(pkt, 1, MPI_PAKIET_T,i,tag,MPI_COMM_WORLD);
            
        }
    }
    pthread_mutex_unlock(&clockLMut);
}

bool areAllAcksSent(){
    pthread_mutex_lock(&ackMut);
    for( int i=0; i< size; i++){
        if( i != rank){
            if(acksSent[i] == false){
                pthread_mutex_unlock(&ackMut);

                return false;
            }
        }
    }
    pthread_mutex_unlock(&ackMut);
    return true;
}
