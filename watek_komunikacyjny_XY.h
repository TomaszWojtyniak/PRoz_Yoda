#include "main.h"

void* startKomWatek_XY(void* ptr){
    MPI_Status status;
    packet_t recv;
    packet_t send;

    while(TRUE){
        recvPacket(&recv, &status);

        switch(status.MPI_TAG){
            case UZUPELNIONO:
            break;

            case ZWIEKSZAM:
            break;
        }

    }
}
