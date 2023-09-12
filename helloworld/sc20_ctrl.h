#ifndef SC20_CTRL_H_
#define SC20_CTRL_H_

#include <stdbool.h>

// Screen Related APIs
int SC20ScreenCtrlInit(void);
int SC20TurnOnScreen(void);
int SC20TurnOffScreen(void);
int SC20ScreenCtrlDeInit(void);

// Ethernet Related APIs
// Library: https://doc.qt.io/qt-5/qnetworkinterface.html
/*
 List of network inteface name:
 Ethernet 1: eth0 - QNetworkInterface::Ethernet
 Ethernet 2: eth1 - QNetworkInterface::Ethernet
 4G LTE: rmnet_data0 - QNetworkInterface::Phonet (maybe ?? need to re-check )
*/

// RADIO Related APIs
int SC20ModemAPIInit(void);
// int SC20ModemSendInitCommand(void);
int SC20ModemSetupDataCall(int sim_slot);
int SC20ModemGetRSSI();

// SIM Card API - not yet tested
int GetSimCardSlotId(void);
int SetSimCardSlotId(int simId);
bool SC20ModemCheckActiveDataCall(void);

#endif