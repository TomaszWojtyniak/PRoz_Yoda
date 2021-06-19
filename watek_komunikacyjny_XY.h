#include "main.h"

void* startKomWatek_XY(void* ptr){
    MPI_Status status;
    packet_t recv;
    packet_t send;

    while(TRUE){
        recvPacket(recv, status);

        switch(status.MPI_TAG){
            case UZUPELNIONO:
                changeState(PAIRED_XY);
            break;

            case ZWIEKSZAM:
                pthread_mutex_lock(&energyMut);
                E += 1;
                pthread_mutex_unlock(&energyMut);
            break;

            case ZMNIEJSZAM:
                pthread_mutex_lock(&energyMut);
                debug("[XY]Dostałem komunikat o zmniejszeniu energii, aktualizje swoją lokalna zmienna  E = %d", E);

                E -= 1;
                debug("[XY] Energia po zmniejszeniu recv.E = %d  E = %d",recv.E, E);

                pthread_mutex_unlock(&energyMut);

                // if(checkEnergy()== 1){ //energia pusta
                //     debug("[XY] ENERGIA PUSTA");
                //     sendPacketToAll(&recv, BRAK_ENERGI);

                //     // isFilled = TRUE;
                // }
                
            break;

            case DO_SEKCJI:
                debug("[XY] JAZDA Z KOLEJKAMI id_dodaj = %d",recv.src);
                pthread_mutex_lock(&waitQueueMut);
                waitQueue.insert(recv.src, recv.zegar, waitQueue.getFirst());
                pthread_mutex_unlock(&waitQueueMut);

                sendPacket(&send, recv.src, ACK);
            break;

            case ACK:

                pthread_mutex_lock(&ackMut);
                debug("[XY] daje zgode ack id_zgoda = %d", recv.src);
                updateClock(recv.zegar);
                acksSent[recv.src] = true;
                pthread_mutex_unlock(&ackMut);
            break;

            case WYCHODZE_XY:
            debug("[XY] usuwam wychodzacego z kolejki id_usun = %d", recv.src);
                pthread_mutex_lock(&waitQueueMut);
                waitQueue.remove(recv.src, waitQueue.getFirst());
                increaseClock(1);
                pthread_mutex_unlock(&waitQueueMut);
            break;

            case BRAK_ENERGI:
                changeState(REST_XY);
                increaseClock(1);
            break;



        }
    }
}
