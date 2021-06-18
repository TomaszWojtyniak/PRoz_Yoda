#include "main.h"

void mainLoop_Z()
{
    packet_t pakiet;

    while (TRUE) {
        if(stan == REST_Z){
            ;
        }
        else if(stan == WAIT_Z){

            debug("Jestem w WAIT_Z, Sprawdzam energie %d", checkEnergy());

            if(checkEnergy() != 0 && isFilled == TRUE){ //energia nie pelna
                
                
                
                
                debug("energia pusta, wchodze do sekcji");

                pthread_mutex_lock( &stateMut);

                changeState(INSECTION_Z);
                pthread_mutex_unlock( &stateMut);
            }
        

            

        } else if (stan == INSECTION_Z){
            debug("Poziom energi przed uzupełnieniem %d",pakiet.E);
           

            changeE(&pakiet);
            
            
            debug("Uzupelnilem energie do %d",pakiet.E);
            if(checkEnergy()== 0){ //energia pelna
                sendPacketToAll(&pakiet, UZUPELNIONO);
                // isFilled = TRUE;
            }
            else{
                isFilled = FALSE;
            }

            
            pthread_mutex_lock( &stateMut);

            changeState(REST_Z);
            pthread_mutex_unlock( &stateMut);

        }

    }
    
}