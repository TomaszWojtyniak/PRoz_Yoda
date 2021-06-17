#include "main.h"
#include "watek_komunikacyjny.h"

void *startKomWatek(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while ( TRUE ) {
	debug("czekam na recv");
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
       pthread_mutex_lock(&clockLMut);
		if(zegar > pakiet.ts)
			zegar++;
		else
			zegar = pakiet.ts+1;
        pthread_mutex_unlock(&clockLMut);
        structElement_t element;
        pthread_mutex_lock(&stateMut);

        switch ( status.MPI_TAG ) {
            case SZUKAM_PARY:
                debug("Dostalem SZUKAM_PARY od %d. ", pakiet.src);
                sendPacket(0,pakiet.src, LACZE_PARE);
                break;
            case LACZE_PARE:
                debug("DOstalem LACZE_PARE od %d", pakiet.src);
                if (stan == ALONE){
                    pairCounter++;
                    if(pairCounter == size-1){
                        pairCounter = 0;
                        //int pairPosition = findProcess(&structQueue, rank);
                        int pairPosition = 3;
                        //if(dostepne polaczenie){
                            para = structQueue.data[pairPosition-1].process;
                            debug("Moija para jest %d", para);
                            sendPacket(0, para, W_PARZE);
                            //removeNFirstElements(&structQueue, pariPosition+1);
                            debug("Zmieniam stan na PAIR");
                            changeState(PAIR);
                        //} else {
                        //   //removeNFirstElements(&structQueue, pariPosition+1);
                        //     debug("Zmieniam stan na WAIT");
                        //     changeState(WAIT);

                        // }
                    }
                }

                break;
            case W_PARZE:
                debug("Dostalem W_PARZE od %d", pakiet.src);
                if(stan == WAIT){
                    para = pakiet.src;
                    debug("Moja para jest %d", para);
                   // removeProcess(&structQueue, rank);
                   // removeProcess(&structQueue, para);
                    debug("Zmieniam stan na PAIR");
                    changeState(PAIR);
                }

                break;
	    default:
	    break;
        }
        pthread_mutex_unlock(&stateMut);
    }
}
