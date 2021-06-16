#ifndef GLOBALH
#define GLOBALH

#define _GNU_SOURCE
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "structs.h"

/* odkomentować, jeżeli się chce DEBUGI */
#define DEBUG 
/* boolean */
#define TRUE 1
#define FALSE 0

#define STATE_CHANGE_PROB 50
#define SEC_IN_STATE 2
//sprawdz co to robi
#define LOOSE_TIME 5

#define ROOT 0

typedef enum {INIT, REST, REST_Z, ALONE, WAIT, SINGLE, PAIR, INSECTION_Z, RANK, INTERSECTION} state_t;
extern state_t stan;
extern int rank;
extern int size;
extern int zegar;
extern int pairCounter;
extern int para;
extern int E;
extern int which;

extern struct_t structQueue;
extern struct_t waitQueue;

extern pthread_mutex_t stateMut;
extern pthread_mutex_t clockLMut;

/* to może przeniesiemy do global... */
typedef struct {
    int ts;       /* timestamp (zegar lamporta */
    int src;      /* pole nie przesyłane, ale ustawiane w main_loop */

    int data;     /* przykładowe pole z danymi; można zmienić nazwę na bardziej pasującą */
    int master;
} packet_t;
extern MPI_Datatype MPI_PAKIET_T;

/* Typy wiadomości */
#define SZUKAM_PARY 1
#define LACZE_PARE 2
#define W_PARZE 3
#define ZWIEKSZAM 4


/* macro debug - działa jak printf, kiedy zdefiniowano
   DEBUG, kiedy DEBUG niezdefiniowane działa jak instrukcja pusta 
   
   używa się dokładnie jak printfa, tyle, że dodaje kolorków i automatycznie
   wyświetla rank

   w związku z tym, zmienna "rank" musi istnieć.

   w printfie: definicja znaku specjalnego "%c[%d;%dm [%d]" escape[styl bold/normal;kolor [RANK]
                                           FORMAT:argumenty doklejone z wywołania debug poprzez __VA_ARGS__
					   "%c[%d;%dm"       wyczyszczenie atrybutów    27,0,37
*/
#ifdef DEBUG
#define debug(FORMAT,...) printf("%c[%d;%dm [%d]: " FORMAT "%c[%d;%dm\n",  27, (1+(rank/7))%2, 31+(6+rank)%7, rank, ##__VA_ARGS__, 27,0,37);
#else
#define debug(...) ;
#endif

#define P_WHITE printf("%c[%d;%dm",27,1,37);
#define P_BLACK printf("%c[%d;%dm",27,1,30);
#define P_RED printf("%c[%d;%dm",27,1,31);
#define P_GREEN printf("%c[%d;%dm",27,1,33);
#define P_BLUE printf("%c[%d;%dm",27,1,34);
#define P_MAGENTA printf("%c[%d;%dm",27,1,35);
#define P_CYAN printf("%c[%d;%d;%dm",27,1,36);
#define P_SET(X) printf("%c[%d;%dm",27,1,31+(6+X)%7);
#define P_CLR printf("%c[%d;%dm",27,0,37);

#define println(FORMAT, ...) printf("%c[%d;%dm [%d]: " FORMAT "%c[%d;%dm\n",  27, (1+(rank/7))%2, 31+(6+rank)%7, rank, ##__VA_ARGS__, 27,0,37);

void sendPacket(packet_t *pkt, int destination, int tag);
void changeState( state_t );
#endif
