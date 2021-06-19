#include "main.h"

void* startKomWatek_Z(void* ptr){
    MPI_Status status;
    packet_t recv;
    packet_t send;

    while(TRUE){
        recvPacket(recv, status);

        switch(status.MPI_TAG){
            case UZUPELNIONO:
            isFilled = TRUE;
            //prawdobodnie zmiana stanu
            debug("[Z] Dostałem komunikat o uzupełnieniu");
            break;

            case BRAK_ENERGI:
            
                debug("[Z]Dostałem komunikat o braku energi, wchodze w stan WAIT");
                
                changeState(WAIT_Z);
                
            break;

            case ZWIEKSZAM:
                pthread_mutex_lock(&energyMut);
                debug("[Z]Dostałem komunikat o zwiekszeniu energii, aktualizje swoją lokalna zmienna  E =  %d", E);

                E += 1;
                debug("[Z]Energia po zwiekszeniu recv.E = %d  E = %d",recv.E, E);


                pthread_mutex_unlock(&energyMut);
                debug("[Z] Ile energi max = %d, checkEnergy() = %d", size/3,checkEnergy());

                // if(checkEnergy()== 0){ //energia pelna
                //     debug("[Z]ENERGIA PELNA");
                //     sendPacketToAll(&recv, UZUPELNIONO);

                
                // }
                



            break;

            case ZMNIEJSZAM:

                debug("[Z]Dostałem komunikat o zmniejszeniu energii, aktualizje swoją lokalna zmienna  E = %d", E);

                pthread_mutex_lock(&energyMut);

                E -= 1;

                debug("[Z]Energia po zmniejszeniu recv.E = %d  E = %d",recv.E, E);

                pthread_mutex_unlock(&energyMut);

                
            break;


        }

    }
}
