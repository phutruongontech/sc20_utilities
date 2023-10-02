	
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <RilPublic.h> // For Radio API

#include "c_printf.h" // For Debug purpose

#include "sc20_ctrl.h"

#include <ql_gpio/ql_gpio.h> // For GPIO

//========================================= SCREEN RELATED APIs ================================
int screen_handle = 0;
int SC20TurnOnScreen(void);
int SC20UserGpioGet(uint32_t gpio_number);
int SC20UserGpioSet(uint32_t gpio_number, uint16_t inout_val);

int SC20ScreenCtrlInit(void)
{
	int ret = 0;

	ret = QLSCREEN_Init(&screen_handle);
	if (ret < 0)
		c_printf("[r]%s%d\n", "QLSCREEN_Init failure:", ret);
	else
	{
		c_printf("[g]%s\n", "QLSCREEN_Init success");
		SC20TurnOnScreen();
	}

	return ret;
}

int SC20TurnOnScreen(void)
{
	int ret = 0;

	ret = QLSCREEN_PowerOn(screen_handle);
	if (ret < 0)
		c_printf("[r]%s%d\n", "QLSCREEN_PowerOn failure:", ret);
	else
		c_printf("[g]%s\n", "QLSCREEN_PowerOn success");

	return ret;
}

int SC20TurnOffScreen(void)
{
	int ret = 0;

	ret = QLSCREEN_PowerOff(screen_handle);
	if (ret < 0)
		c_printf("[r]%s%d\n", "QLSCREEN_PowerOff failure:", ret);
	else
		c_printf("[g]%s\n", "QLSCREEN_PowerOff success");

	return ret;
}

int SC20ScreenCtrlDeInit(void)
{
	int ret = 0;

	ret = QLSCREEN_Exit(&screen_handle);
	if (ret < 0)
		c_printf("[r]%s%d\n", "QLSCREEN_Exit failure:", ret);
	else
		c_printf("[g]%s\n", "QLSCREEN_Exit success");

	return ret;
}

//========================================= NETWORK RELATED APIs ================================
int modem_handle = 0;

// const char *startup_at_cmd[] = {
// 		"ATI",
// 		"AT+CPIN?",
// 		"AT+COPS?",
// 		"AT$QCSIMAPP=?",
// 		"AT+QFSGVERSION?",
// 		"AT+QGMR",
// 		"AT+CSUB",
// 		"AT+QMBNCFG=\"list\"",
// 		"at+cimi",
// 		"at+iccid",
// 		"at+egmr=0,7",
// 		"at+egmr=0,10",
// 		"at+egmr=0,5",
// 		"AT+CSQ",
// 		"AT+CREG?",
// 		"AT^SYSCONFIG?",
// 		"AT+CGSN",
// 		"AT+CCLK?",
// 		"AT+CGDCONT?",
// 		"AT+CGPADDR",
// 		"AT+ICCID",
// 		NULL,
// };


// int SC20ModemSendInitCommand(void)
// {
// 	int i;
// 	int ret = 0;
// 	char cmd_resp[1024] = {0};

// 	for (i = 0; startup_at_cmd[i] != NULL; i++) {
// 		memset(cmd_resp, 0, sizeof(cmd_resp));
// 		ret = QLMISC_SendAT(startup_at_cmd[i], cmd_resp, sizeof(cmd_resp));
// 		if (ret < 0)
// 			c_printf("[r]%s%d\n", "QLMISC_SendAT failed with return:", ret);
// 		else {
// 			c_printf("AT command response[%d]:\n", ret);
// 			c_printf("[g]%s\n", cmd_resp);
// 		}
// 	}

// 	return ret;
// }



int SC20ModemSendATCommand(const char* user_at_cmd)
{
	int i;
	int ret = 0;
	char cmd_resp[1024] = {0};

	{
		memset(cmd_resp, 0, sizeof(cmd_resp));
		ret = QLMISC_SendAT(user_at_cmd, cmd_resp, sizeof(cmd_resp));
		if (ret < 0)
			c_printf("[r]%s%d\n", "QLMISC_SendAT failed with return:", ret);
		else {
			c_printf("AT command response[%d]:\n", ret);
			c_printf("[g]%s\n", cmd_resp);
		}
	}

	return ret;
}

/*
Response: "+CSQ: 19,0"
Value	RSSI dBm	Condition
10	-93	OK
11	-91	OK
12	-89	OK
13	-87	OK
14	-85	OK
15	-83	Good
16	-81	Good
17	-79	Good
18	-77	Good
19	-75	Good
20	-73	Excellent
21	-71	Excellent
22	-69	Excellent
23	-67	Excellent
24	-65	Excellent
25	-63	Excellent
26	-61	Excellent
27	-59	Excellent
28	-57	Excellent
29	-55	Excellent
30	-53	Excellent
*/

int SC20ModemGetRSSI()
{
	int ret = 0;
	RIL_SignalStrength_v10 *ss = NULL;
	int xRetRssi = 0;

	ret = QLRIL_GetSignalStrength(&modem_handle, (void **)&ss);
	if (ret < 0)
		printf("%s%d\n", "QLRIL_GetSignalStrength with return:", ret);
	else {
		/**
		 * UINT_MAX = 2147483647
		 */
		if (ss != NULL) {
			printf("[signalStrength=%d, bitErrorRate=%d, "
					"CDMA_SS.dbm=%d, CDMA_SSecio=%d, "
					"EVDO_SS.dbm=%d, EVDO_SS.ecio=%d,"
					"EVDO_SS.signalNoiseRatio=%d,\n"
					"LTE_SS.signalStrength=%d, LTE_SS.rsrp=%d, LTE_SS.rsrq=%d, "
					"LTE_SS.rssnr=%d, LTE_SS.cqi=%d, TDSCDMA_SS.rscp=%d]\n",
					ss->GW_SignalStrength.signalStrength,
					ss->GW_SignalStrength.bitErrorRate,
					ss->CDMA_SignalStrength.dbm,
					ss->CDMA_SignalStrength.ecio,
					ss->EVDO_SignalStrength.dbm,
					ss->EVDO_SignalStrength.ecio,
					ss->EVDO_SignalStrength.signalNoiseRatio,
					ss->LTE_SignalStrength.signalStrength,
					ss->LTE_SignalStrength.rsrp,
					ss->LTE_SignalStrength.rsrq,
					ss->LTE_SignalStrength.rssnr,
					ss->LTE_SignalStrength.cqi,
					ss->TD_SCDMA_SignalStrength.rscp);
			xRetRssi = ss->GW_SignalStrength.signalStrength;
			free(ss);
			ss = NULL;
		}
	}

	return xRetRssi;
}

bool SC20ModemCheckActiveDataCall(void)
{
	int ret = 0;
	bool isNetOK = false;
	int i = 0;
	char buf[512] = {0};

	RIL_Data_Call_Response_v11 *dataCallList = NULL;

	if ( modem_handle == 0)
	{
		printf("Invalid handle\n");
		return false;
	}
	ret = QLRIL_GetDataCallList(&modem_handle, (void **)&dataCallList);

	if (ret > 0) 
	{
		c_printf ("[g]%s[y]%d\n", "QLRIL_GetDataCallList success return items: ", ret);
		if (dataCallList == NULL) 
		{
			c_printf("[r]%s\n", "QLRIL_GetDataCallList pointer is NULL");
			return -1;
		}

		for (i = 0; i < ret; i++) 
		{
			c_printf ("[b]%s\n", "GetDataCallList response:");
			snprintf (buf, sizeof(buf), "status:%d, retry:%d, call_id:%d, active:%s\n"
					"type:%s, ifname:%s, addresses:%s, dnses:%s\n"
					"gateways:%s, pcscf:%s, mtu:%d\n",
					dataCallList[i].status, dataCallList[i].suggestedRetryTime,
					dataCallList[i].cid, (dataCallList[i].active == 0)?"down":"up",
					dataCallList[i].type, dataCallList[i].ifname, dataCallList[i].addresses,
					dataCallList[i].dnses, dataCallList[i].gateways,
					dataCallList[i].pcscf, dataCallList[i].mtu);
			buf[sizeof(buf) - 1] = 0;//prevent stack overflows
			c_printf("[g]%s\n", buf);

			if ( dataCallList[i].status == 0 )
			{
				printf("Data call started success!!");
				isNetOK = true;
				break;
			}

			if (dataCallList[i].type)
				free(dataCallList[i].type);
			if (dataCallList[i].ifname)
				free(dataCallList[i].ifname);
			if (dataCallList[i].addresses)
				free(dataCallList[i].addresses);
			if (dataCallList[i].dnses)
				free(dataCallList[i].dnses);
			if (dataCallList[i].gateways)
				free(dataCallList[i].gateways);
			if (dataCallList[i].pcscf)
				free(dataCallList[i].pcscf);
		}
	} 

	return isNetOK;
}

#define DEFAULT_APN 		"m-wap"
#define DEFAULT_APN_USR     "mms"
#define DEFAULT_APN_PWD     "mms"

int SC20ModemSetupDataCall(int sim_slot)
{
	char buf[512] = {0};
	int xRet = -1;
	int on = 0;
	int slotId = 0;
	int fool = 0;
	int type = 0;
	int cmdIdx = 0;
	int start, end, i, num;

	char ch = 0;
	char areaCode[10] = {0};
	char addr[30] = {0};
	char msg[200] = {0};

	int radioTech = 14;
	int profileId = 0;
	int ret = 0;

	char *str = NULL;
	char dnses[200] = {0};
	char buffer[100] = {0};
	RIL_Data_Call_Response_v11 *dataCall = NULL;

	if ( SetSimCardSlotId(sim_slot) != 0)
	{
		printf("Swith sim slot failed\n");
		return -1;
	}

	if (modem_handle == 0)
	{
		printf("Invalid modem handle\n");
		return -1;
	}

	sleep(3);

	ret = QLRIL_SetupDataCall(&modem_handle, radioTech, profileId, DEFAULT_APN, DEFAULT_APN_USR, DEFAULT_APN_PWD, 0, "IPV4V6", (void **)&dataCall);

	if (ret > 0 && dataCall != NULL) 
	{
	c_printf ("[b]%s%d%s\n", "setupDataCall response[", ret, "]:");
	snprintf (buf, sizeof(buf), "status:%d, retry:%d, callID:%d, active:%s\n"
			"type:%s, ifname:%s, addresses:%s, dnses:%s\n"
			"gateways:%s,pcscf:%s,mtu:%d\n",
			dataCall->status, dataCall->suggestedRetryTime,
			dataCall->cid, (dataCall->active == 0)?"down":"up",
			dataCall->type, dataCall->ifname, dataCall->addresses,
			dataCall->dnses, dataCall->gateways, dataCall->pcscf, dataCall->mtu);
	buf[sizeof(buf) - 1] = 0;//prevent stack overflows
	xRet = dataCall->status;
	c_printf("[g]%s\n", buf);

	if (dataCall->active == 0) {
		c_printf("[r]%s\n", "Network is not active, may be you can try 'others' mode");
	} else {
		if (dataCall->dnses != NULL && strlen(dataCall->dnses) > 0) {//FIXME maybe
			if(strlen(dataCall->dnses) > 31) {
				ret = snprintf(dnses, sizeof(dnses), "%s", "2400:3200::1");
				ret += snprintf(dnses + ret, sizeof(dnses) - ret, " %s", "2400:3200:baba::1");
				strcpy(buffer, dataCall->dnses);
				ret += snprintf(dnses + ret, sizeof(dnses) - ret, " %s", buffer);
				str = dnses;
			} else
				str = dataCall->dnses;
		} else {
			ret = snprintf(dnses, sizeof(dnses), "%s", "8.8.8.8");//for example
			ret += snprintf(dnses + ret, sizeof(dnses) - ret, " %s", "4.2.2.2");
			ret += snprintf(dnses + ret, sizeof(dnses) - ret, " %s", "2400:3200::1");
			ret += snprintf(dnses + ret, sizeof(dnses) - ret, " %s", "2400:3200:baba::1");
			str = dnses;
		}

		ret = QLRIL_AppendDNSForIPV4V6(str);
		if (ret <= 0)
			c_printf("[r]%s%d\n", "QLRIL_SetDNSForIPV4V6 failure return:", ret);
		else
			c_printf ("[g]%s\n", "QLRIL_SetDNSForIPV4V6 success");

		if (dataCall->ifname != NULL && strlen(dataCall->ifname) > 0) {
			ret = QLRIL_SetRouteForIPV4V6(dataCall->ifname);
			if (ret < 0)
				c_printf("[r]%s%d\n", "QLRIL_SetRouteForIPV4V6 failed with return:", ret);
			else
				c_printf ("[g]%s\n", "QLRIL_SetRouteForIPV4V6 success");
		} else
			c_printf ("[g]%s\n", "QLRIL_SetupDataCall called failed without interface name");
	}

	if (dataCall->type)
		free(dataCall->type);
	if (dataCall->ifname)
		free(dataCall->ifname);
	if (dataCall->addresses)
		free(dataCall->addresses);
	if (dataCall->dnses)
		free(dataCall->dnses);
	if (dataCall->gateways)
		free(dataCall->gateways);
	if (dataCall->pcscf)
		free(dataCall->pcscf);

	if (dataCall) {
		free(dataCall);
		dataCall = NULL;
	}
	} else {
	c_printf ("[r]%s%d\n", "QLRIL_SetupDataCall failed with return:", ret);
	}

	return xRet;
}

int SC20ModemAPIInit(void)
{
	int ret = 0;
	int i = 0;

	if (modem_handle == 0) 
	{
		for (i = 0; i < 5; i++) 
		{
			ret = QLRIL_Init(&modem_handle);
			if (ret == 0) {
				c_printf("[g]%s\n", "QLRIL_Init success");
				break;
			}
			sleep(1);
		}

		if (modem_handle == 0 || i >= 5) 
		{
			c_printf("[r]%s%d\n", "QLRIL_Init failed with return:", ret);
		}
	}

	return ret;
}


/* NOT YET READY */
int GetSimCardSlotId()
{
	int ret;
	int slot = 0;

	ret = QLRIL_GetSimCardSlotId(&modem_handle, &slot);
	if (ret == 0) {
		c_printf("[g]%s%d\n", "Current SIM card slot ID: ", slot);
	} else
		c_printf("[r]%s%d\n", "QLRIL_GetSimCardSlotId failed with return:", ret);

	return slot;
}

/* NOT YET READY */
int  SetSimCardSlotId(int sim_id)
{
	int ret = 0;
	int curr_slot = 0;

	ret = QLRIL_SetSimCardSlotId(&modem_handle, sim_id);
	if (ret == 0)
	c_printf("[g]%s\n", "QLRIL_SetSimCardSlotId success");
	else
	c_printf("[r]%s%d\n", "QLRIL_SetSimCardSlotId failed with return: ", ret);
	
	curr_slot = GetSimCardSlotId();
	if (curr_slot != sim_id)
	{
		printf("Switch sim slot failed\n");
		return -1;
	}

	// SC20ModemSendATCommand("at$qcsimapp=1");

	return 0;
}


//========================================= GPIO RELATED APIs ================================
int SC20UserGpioInit()
{
	#define GPIO_OUTPUT_DEFAULT_CONFIG      (0x203)

	QL_Gpio_Set_Config(SC20_GPIO_LED_ERR, GPIO_OUTPUT_DEFAULT_CONFIG);
	QL_Gpio_Set_Config(SC20_GPIO_LED_SD, GPIO_OUTPUT_DEFAULT_CONFIG);
	QL_Gpio_Set_Config(SC20_GPIO_LED_LTE_4G, GPIO_OUTPUT_DEFAULT_CONFIG);
	QL_Gpio_Set_Config(SC20_GPIO_LED_DATA, GPIO_OUTPUT_DEFAULT_CONFIG);
	QL_Gpio_Set_Config(SC20_GPIO_PWR_USB_HUB, GPIO_OUTPUT_DEFAULT_CONFIG);
	QL_Gpio_Set_Config(SC20_GPIO_PWR_USB_OTG, GPIO_OUTPUT_DEFAULT_CONFIG);
	QL_Gpio_Set_Config(SC20_GPIO_PWR_USB_BUS, GPIO_OUTPUT_DEFAULT_CONFIG);

	return 0;
}


int SC20UserGpioGet(uint32_t gpio_number)
{
	uint32_t inout_val = 0;

	if (!Gpio_Valid(gpio_number)) {
		fprintf(stderr, "Invalid gpio number: %d\n", gpio_number);
		return -1;
	}
	QL_Gpio_Get_Level(gpio_number, &inout_val);
}

int SC20UserGpioSet(uint32_t gpio_number, uint16_t inout_val)
{
	if (!Gpio_Valid(gpio_number)) {
		fprintf(stderr, "Invalid gpio number: %d\n", gpio_number);
		return -1;
	}

	QL_Gpio_Set_Level(gpio_number, inout_val);

	return 0;
}