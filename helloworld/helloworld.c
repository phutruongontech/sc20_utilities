#include <stdio.h>
#include "qlril_ctrl.h"

int main(int argc, char **argv)
{
    printf("Init API \n");
    InitQLRILApi();

    sleep(1);
    printf("Set sim slot \n");
    SetSimCardSlotId(2);
    
    GetSimCardSlotId();

    sleep(5);
    printf("Send init at command \n");
    SetupInitATCmd();

    sleep(1);
    printf("Start data call \n");
    SetupDataCall();

    return 0;
}

