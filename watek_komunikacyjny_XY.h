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
                E -= 1;
                pthread_mutex_unlock(&energyMut);
            break;

            case DO_SEKCJI:
                pthread_mutex_lock(&waitQueueMut);
                waitQueue.insert(recv.src, recv.zegar, waitQueue.getFirst());
                pthread_mutex_unlock(&waitQueueMut);

                sendPacket(&send, recv.src, ACK);
            break;

            case ACK:

                pthread_mutex_lock(&ackMut);
                debug("daje zgode ack");
                acksSent[recv.src] = true;
                pthread_mutex_unlock(&ackMut);
            break;

            case WYCHODZE_XY:
            debug("usuwam wychodzacego z kolejki");
                pthread_mutex_lock(&waitQueueMut);
                waitQueue.remove(recv.src, waitQueue.getFirst());
                pthread_mutex_unlock(&waitQueueMut);
            break;
        }

    }
}
