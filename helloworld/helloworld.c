#include <stdio.h>
#include "sc20_ctrl.h"

/*
mount -o remount,rw /
*/
void ScreenTest(void);
void ModemLTE_Test(int);

int main(int argc, char **argv)
{
    int sim_lot = 0;

    if (argc < 2)
    {
        printf("Please enter sim slot index (1, 2)\n");
        return -1;
    }
    sim_lot = atoi(argv[1]);
    printf("Using sim lot %d \n", sim_lot);

    ModemLTE_Test(sim_lot);
    ScreenTest();
    
    return 0;
}


void ScreenTest(void)
{
    int i = 0;
    SC20ScreenCtrlInit();

   // for (i=0; i<3; i++)
    {
        SC20TurnOffScreen();
        sleep(5);
        SC20TurnOnScreen();
    }
}

void ModemLTE_Test(int sim_lot)
{
    int rssi = 0;
    int net_status = -1;

    SC20ModemAPIInit();

    net_status = SC20ModemSetupDataCall(sim_lot);
    if ( net_status == 0)
    {   
        printf("NETWORK CONNECTED!!\n");
    }
    
    rssi = SC20ModemGetRSSI();
    printf("Current RSSI: %d\n", rssi);    
}