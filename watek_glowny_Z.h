#include "main.h"

void mainLoop_Z()
{
    packet_t pakiet;

    while (TRUE) {
        if(stan == REST_Z){
            if(checkEnergy() != 0){ //energia nie pelna
                debug("energia pusta, wchodze do sekcji");

                pthread_mutex_lock( &stateMut);

                changeState(INSECTION_Z);
                pthread_mutex_unlock( &stateMut);
            }

        } else if (stan == INSECTION_Z){
            pthread_mutex_lock( &stateMut);

            changeE(&pakiet);
            debug("Uzupelnilem energie do %d",pakiet.E);
            if(checkEnergy()== 0){ //energia pelna
                sendPacketToAll(&pakiet, UZUPELNIONO);
            }
            changeState(REST_Z);
            pthread_mutex_unlock( &stateMut);

        }

    }
    
}