#include "main.h"


void mainLoop_XY()
{
    packet_t pakiet;

    while (TRUE) {
        if(stan == REST_XY){
            debug("jestem w restXY");
            changeState(PAIRED_XY);

        } else if (stan == PAIRED_XY){
            pthread_mutex_lock(&ackMut);
            debug("jestem sparowany XY");
            for(int i=0;i< size;i++){
                if(i != rank){
                    acksSent[i] = false;
                }
            }
            pthread_mutex_unlock(&ackMut);
            sendPacketToAllAndAddMeToSectionQueue(&pakiet, DO_SEKCJI );
            
            changeState(WAIT_XY);


        } else if (stan == WAIT_XY){
            if(areAllAcksSent()){
                pthread_mutex_lock(&ackMut);
                debug("jestem sparowany WAIT_XY");

                pthread_mutex_lock(&waitQueueMut);
                waitQueue.remove(rank,waitQueue.getFirst());
                pthread_mutex_unlock(&waitQueueMut);
                sleep(SLEEP_TIME);
                for (int i=0; i< size;i++){
                    if( rank != rank){
                        acksSent[i] = false;
                    }
                }
                pthread_mutex_unlock(&ackMut);
                changeState(INSECTION_XY);
            }

        } else if (stan == INSECTION_XY){



            debug("Wchodze do sekcji");

            pthread_mutex_lock(&energyMut);
            debug("Poziom energi przed zabraniem %d",pakiet.E);

            decreaseE(&pakiet);
            if(checkEnergy() == 1){
                sendPacketToAll(&pakiet, BRAK_ENERGI);
            }

            debug("Obniżam energie do %d",pakiet.E);
            pthread_mutex_unlock(&energyMut);

        }

    }
    
}