#include "main.h"


void mainLoop_XY()
{
    packet_t pakiet;

    while (TRUE) {
        if(stan == REST_XY){

            debug("Ile jest energii %d", E);
            sleep(SLEEP_TIME);

        } else if (stan == PAIRED_XY){
            pthread_mutex_lock(&ackMut);
            for(int i=0;i< size;i++){
                if(i != rank){
                    acksSent[i] = false;
                }
            }
            pthread_mutex_unlock(&ackMut);
            debug("jestem w PAIRED)XY i wysylam req do sekcji");
            sendPacketToAllAndAddMeToSectionQueue(&pakiet, DO_SEKCJI );
            
            changeState(WAIT_XY);


        } else if (stan == WAIT_XY){
            if(areAllAcksSent()){
                pthread_mutex_lock(&ackMut);
                debug("jestem w WAIT_XY i dostalem wszystkie ack");

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
            //debug("jestem w wait_xy i czekam na ack");
            sleep(SLEEP_TIME);

        } else if (stan == INSECTION_XY){



            debug("Wchodze do sekcji");

            
            debug("Poziom energi przed zabraniem %d",E);

            decreaseE(&pakiet);

            debug("Obniżam energie do %d",E);
            

            //sendPacketToAll(&pakiet, WYCHODZE_XY);
            changeState(REST_XY);

        }

    }
    
}