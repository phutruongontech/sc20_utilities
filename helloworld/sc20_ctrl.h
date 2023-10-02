#ifndef SC20_CTRL_H_
#define SC20_CTRL_H_

#include <stdbool.h>

typedef enum 
{
    SC20_GPIO_LED_ERR=110,
    SC20_GPIO_LED_SD=98,
    SC20_GPIO_LED_LTE_4G=68,
    SC20_GPIO_LED_DATA=97,
    SC20_GPIO_PWR_USB_HUB=94,
    SC20_GPIO_PWR_USB_OTG=8,
    SC20_GPIO_PWR_USB_BUS=65,
    SC20_GPIO_MAX,
}SC20_GPIO_Enum_t;

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

// GPIO API
int SC20UserGpioInit();
int SC20UserGpioGet(uint32_t gpio_number);
int SC20UserGpioSet(uint32_t gpio_number, uint16_t inout_val);

#endif