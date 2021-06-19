#include "main.h"

void mainLoop_Z()
{
    packet_t pakiet;

    while (TRUE)
    {
        if (stan == REST_Z)
        {
            debug("[Z] Jestem w REST_Z ILE MAM ENERGI PO WSZYSTKIM   E =  %d", E);
            sleep(SLEEP_TIME);
        }
        else if (stan == WAIT_Z)
        {

            debug("[Z]Jestem w WAIT_Z, Sprawdzam energie E= %d checkEnergy() = %d i isFilled =  %d",E, checkEnergy(), isFilled);
            sleep(SLEEP_TIME);
            if (checkEnergy() != 0 && isFilled == TRUE)
            { //energia nie pelna

                debug("[Z] Jestem w WAIT_Z energia nie pelna, wchodze do sekcji E = %d", E);

                changeState(INSECTION_Z);
            }
        }
        else if (stan == INSECTION_Z)
        {
            debug("[Z] Jestem w sekcji Poziom energi przed uzupełnieniem E = %d", E);

            changeE(&pakiet);

            debug("[Z] Udało się uzupełnic E =  %d", E)

            changeState(REST_Z);
        }
    }
}