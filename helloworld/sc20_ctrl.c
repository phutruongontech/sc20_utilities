	
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#include <RilPublic.h>
#include "c_printf.h"

const char *cmd[] = {
		"ATI",
		"AT+CPIN?",
		"AT+COPS?",
		"AT$QCSIMAPP=?",
		"AT+QFSGVERSION?",
		"AT+QGMR",
		"AT+CSUB",
		"AT+QMBNCFG=\"list\"",
		"at+cimi",
		"at+iccid",
		"at+egmr=0,7",
		"at+egmr=0,10",
		"at+egmr=0,5",
		"AT+CSQ",
		"AT+CREG?",
		"AT^SYSCONFIG?",
		"AT+CGSN",
		"AT+CCLK?",
		"AT+CGDCONT?",
		"AT+CGPADDR",
		"AT+ICCID",
		NULL,
};

int handle = 0;

//========================================= SCREEN RELATED APIs ================================
int screen_handle = 0;

int SC20ScreenCtrlInit(void)
{
	int ret = 0;

	ret = QLSCREEN_Init(&screen_handle);
	if (ret < 0)
		c_printf("[r]%s%d\n", "QLSCREEN_Init failure:", ret);
	else
		c_printf("[g]%s\n", "QLSCREEN_Init success");

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

void SC20TurnOffScreen(void)
{
	int ret = 0;

	ret = QLSCREEN_PowerOff(screen_handle);
	if (ret < 0)
		c_printf("[r]%s%d\n", "QLSCREEN_PowerOff failure:", ret);
	else
		c_printf("[g]%s\n", "QLSCREEN_PowerOff success");

	return ret;
}

int SetupInitATCmd(void)
{
	int i;
	int ret = 0;
	char cmd_resp[1024] = {0};

	for (i = 0; cmd[i] != NULL; i++) {
		memset(cmd_resp, 0, sizeof(cmd_resp));
		ret = QLMISC_SendAT(cmd[i], cmd_resp, sizeof(cmd_resp));
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
* Radio technology:
* 3 - UMTS
* 14 - LTE
* For more information refer to RIL_RadioTechnology in ril.h
*/

/*
* Profile Id:
* 0 - RIL_DATA_PROFILE_DEFAULT
* 1 - RIL_DATA_PROFILE_TETHERED
* 2 - RIL_DATA_PROFILE_IMS
* 3 - RIL_DATA_PROFILE_FOTA
* 4 - RIL_DATA_PROFILE_CBS
* 1000 - RIL_DATA_PROFILE_OEM_BASE
* 0xFFFFFFFF - RIL_DATA_PROFILE_INVALID
* For more information refer to RIL_DataProfile in ril.h
*/

int SetupDataCall(void)
{
	char buf[512] = {0};

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
	// char apn[40] = {0};
	// char protocol[10] = {0};
	int ret = 0;

	char *str = NULL;
	char dnses[200] = {0};
	char buffer[100] = {0};
	RIL_Data_Call_Response_v11 *dataCall = NULL;

	ret = QLRIL_SetupDataCall(&handle, radioTech, profileId, "", "", "", 0, "IP", (void **)&dataCall);

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
}

int GetSimCardSlotId(void)
{
	int ret;
	int slotId = 0;
	ret = QLRIL_GetSimCardSlotId(&handle, &slotId);
	if (ret == 0) {
	c_printf("[g]%s%d\n", "Current SIM card slot ID: ", slotId);
	} else
	c_printf("[r]%s%d\n", "QLRIL_GetSimCardSlotId failed with return:", ret);
}

void SetSimCardSlotId(int sim_id)
{
	int ret = 0;

	ret = QLRIL_SetSimCardSlotId(&handle, sim_id);
	if (ret == 0)
	c_printf("[g]%s\n", "QLRIL_SetSimCardSlotId success");
	else
	c_printf("[r]%s%d\n", "QLRIL_SetSimCardSlotId failed with return: ", ret);
}



int InitQLRILApi(void)
{
	int ret = 0;
	char buf[512] = {0};
	int i = 0;

	// int on = 0;
	// int slotId = 0;
	// int fool = 0;
	// int type = 0;
	// int cmdIdx = 0;
	// int start, end, i, num;

	// char ch = 0;
	// char areaCode[10] = {0};
	// char addr[30] = {0};
	// char msg[200] = {0};
	

	if (handle == 0) {
		for (i = 0; i < 5; i++) {
			ret = QLRIL_Init(&handle);
			if (ret == 0) {
				c_printf("[g]%s\n", "QLRIL_Init success");
				break;
			}

			sleep(1);
		}

		if (handle == 0 || i >= 5) {
			c_printf("[r]%s%d\n", "QLRIL_Init failed with return:", ret);
		}
	}

	memset(buf, 0, sizeof(buf));
	ret = QLRIL_GetVersion(buf, sizeof(buf));
	if (ret != 0)
		c_printf("[r]%s%d\n", "QLRIL_GetVersion failed with return:", ret);
	else
		c_printf("[g]%s%s\n", "QLRIL library current version:", buf);

	return ret;
}