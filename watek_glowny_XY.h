#include "main.h"


void mainLoop_XY()
{
    packet_t pakiet;

    while (TRUE) {
        if(stan == REST_XY){

            //debug("Ile jest energii %d", E);
            sleep(SLEEP_TIME);

        } else if (stan == PAIRED_XY){
            pthread_mutex_lock(&ackMut);
            for(int i=0;i< size;i++){
                if(i != rank){
                    acksSent[i] = false;
                }
            }
            pthread_mutex_unlock(&ackMut);
            // debug("[XY] jestem w PAIRED_XY i wysylam req do sekcji i dodaje sie do kolejki");
            pthread_mutex_lock(&waitQueueMut);
            waitQueue.insert(rank,zegar, waitQueue.getFirst());
            pthread_mutex_unlock(&waitQueueMut);
            increaseClock(1);
            sendPacketToAll(&pakiet, DO_SEKCJI );
            
            changeState(WAIT_XY);


        } else if (stan == WAIT_XY){
            debug("[XY]jestem w WAIT_XY i  NIE dostalem wszystkie ack");
            sleep(SLEEP_TIME);
            if(areAllAcksSent()){
                pthread_mutex_lock(&ackMut);
                debug("[XY]jestem w WAIT_XY i dostalem wszystkie ack");

                pthread_mutex_lock(&waitQueueMut);
            	std::vector<Data> better;
            	waitQueue.getBetterVector(rank, better, waitQueue.getFirst());
            	printf("[XY] ALL PROCESSES ABOVE PROCESS WITH ID %d:\n", rank);
            	for (int i = 0; i < better.size(); i++)
            	{
            		printf("id=%d, prio=%d\n", better[i].id, better[i].priority);
            	}
                pthread_mutex_unlock(&waitQueueMut);
                sleep(SLEEP_TIME);
                for (int i=0; i< size;i++){
                    if( rank != rank){
                        acksSent[i] = false;
                    }
                }
                pthread_mutex_unlock(&ackMut);
                if(checkEnergy() != 1 && better.size() == 0){
                    debug("[XY] Wchodze do sekcji");
                    changeState(INSECTION_XY);
                }
                //changeState(INSECTION_XY);
            }
            //debug("jestem w wait_xy i czekam na ack");
            sleep(SLEEP_TIME);

        } else if (stan == INSECTION_XY){



            

            
            debug("[XY] Poziom energi przed zabraniem %d",E);

            decreaseE(&pakiet);

            debug("[XY] Poziom energi po zabraniu %d",E);
            
            //pthread_mutex_lock(&waitQueueMut);
            waitQueue.remove(rank, waitQueue.getFirst());

            //pthread_mutex_unlock(&waitQueueMut);
            sendPacketToAll(&pakiet, WYCHODZE_XY);
            changeState(REST_XY);

        }

    }
    
}