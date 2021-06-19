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
                
                changeState(WAIT_Z);
                
            break;

            case ZWIEKSZAM:
                pthread_mutex_lock(&energyMut);
                debug("Dostałem komunikat o zwiekszeniu energii, aktualizje swoją lokalna zmienna  E %d", E);

                E += 1;
                debug("Energia po zwiekszeniu recv.E %d  E %d",recv.E, E);

                pthread_mutex_unlock(&energyMut);

                if(checkEnergy()== 0){ //energia pelna
                    debug("ENERGIA PELNA");
                    sendPacketToAll(&recv, UZUPELNIONO);

                    // isFilled = TRUE;
                }
                else{
                    isFilled = FALSE;
                }



            break;

            case ZMNIEJSZAM:
                pthread_mutex_lock(&energyMut);
                debug("Dostałem komunikat o zmniejszeniu energii, aktualizje swoją lokalna zmienna %d",recv.E);

                E -= 1;

                debug("Energia po zmniejszeniu %d",recv.E);

                pthread_mutex_unlock(&energyMut);
            break;


        }

    }
}
