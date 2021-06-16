#include "main.h"
#include "watek_glowny.h"

void mainLoop()
{
    srandom(rank);
    while (TRUE) {
        if(stan == INIT){
            structElement_t element;
            which = srand(10) %3; // 0 - Z , 1 - X, 2 - Y
            if (which == 0){
                changeE();
            }
            element.master = which
            changeState(REST)
        }
        else if(stan == REST){
            sleep(SEC_IN_STATE);
            int perc = random()%100;
            pthread_mutex_lock( &stateMut ); 
            if (perc<STATE_CHANGE_PROB) {
                debug("Zmieniam stan na ALONE");
                changeState(ALONE);
                int pr = zegar;
                for(int i=0; i< size; i++){
                    if(i != rank){
                        packet_t * pkt = malloc(sizeof(packet_t));
                        pkt->data = pr;
                        sendPacket( pkt, i, SZUKAM_PARY);
                    }
                }
                structElement_t element;
                element.priority = pr;
                element.process = rank;
                debug("Jestem mistrzem %d",element.master);
                //insertElement(&structQueue, element);
                pairCounter = 0;
            }
        pthread_mutex_unlock( &stateMut);

        } else if(stan == INSECTION){
            debug("Jestem z nadhiberprzestrzeni")
            sleep( SEC_IN_STATE);
            pthread_mutex_lock( &stateMut);
            debug("Zmieniam stan na REST");
            changeState(REST);
            //onStartResting();
            pthread_mutex_unlock( &stateMut);

        }

    }

}
