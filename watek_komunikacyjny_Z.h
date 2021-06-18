#include "main.h"

void* startKomWatek_Z(void* ptr){
    MPI_Status status;
    packet_t recv;
    packet_t send;

    while(TRUE){
        recvPacket(recv, status);

        switch(status.MPI_TAG){
            case UZUPELNIONO:
                // recv. = TRUE;
            //prawdobodnie zmiana stanu
            debug("Dostałem komunikat o uzupełnieniu");
            break;

            case BRAK_ENERGI:
                recv.isFilled = TRUE;
                debug("Dostałem komunikat o braku energi, wchodze w stan WAIT");
                pthread_mutex_lock(&stateMut);
                changeState(WAIT_Z);
                pthread_mutex_unlock(&stateMut);
            break;

            case ZWIEKSZAM:
                pthread_mutex_lock(&energyMut);
                debug("Dostałem komunikat o zwiekszeniu energii, aktualizje swoją lokalna zmienna recv.E %d  E %d",recv.E, E);

                recv.E += 1;
                debug("Energia po zwiekszeniu recv.E %d  E %d",recv.E, E);

                pthread_mutex_unlock(&energyMut);
            break;

            case ZMNIEJSZAM:
                pthread_mutex_lock(&energyMut);
                debug("Dostałem komunikat o zmniejszeniu energii, aktualizje swoją lokalna zmienna %d",recv.E);

                recv.E -= 1;

                debug("Energia po zmniejszeniu %d",recv.E);

                pthread_mutex_unlock(&energyMut);
            break;
        }

    }
}
