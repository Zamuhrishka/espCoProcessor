/********************************************************************************
* @author  		Kovalchuk Alexander
* @email  		alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
* 				which use for...
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include <qa_wifi.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "slre.h"
#include "qa_port.h"
//_____ C O N F I G S  ________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
#define  BUFFER_LENGTH             200u
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
extern char txBuffer[BUFFER_SIZE];

static char buffer[BUFFER_LENGTH] = {0};

static char wifiMode[] = "0";

static char AP_sssid[32] = "ArduinoTest";
static char AP_pwd[64] = "1234587";
static char AP_bssid[] = "ca:d7:19:d8:a6:44";
static char AP_channel[] = "3";
static char AP_rssi[] = "-125";
static char AP_pci_en[] = "0";


static char SoftAP_ssid[32] = "ArduinoSoftAP";
static char SoftAP_pwd[64] = "1234587";
static char SoftAP_channel[] = "3";
static char SoftAP_enc[] = "3";

static char Station_mac[] = "18:fe:35:98:d3:7b";
static char SoftAP_mac[] = "1a:fe:36:97:d5:7b";

static char Station_ip[] = "000.000.000.000";
static char Station_gw[] = "000.000.000.000";
static char Station_mask[] = "000.000.000.000";

static char SoftAP_ip[] = "000.000.000.000";
static char SoftAP_gw[] = "000.000.000.000";
static char SoftAP_mask[] = "000.000.000.000";



static char ssid[32] = "ArduinoTest";
static char pwd[64] = "1234587";
static char bssid[] = "ca:d7:19:d8:a6:44";
static char channel[] = "3";
static char rssi[] = "-125";
static char pci_en[] = "0";
static char enc[] = "3";

static char ip[] = "192.168.6.100";
static char gw[] = "192.168.6.1";
static char mask[] = "255.255.255.0";
//----------------------------------------------------------------------------------------------------
static char pattern_OK1[] = "\r\nOK\r\n";
static char pattern_ERROR1[] = "\r\nERROR\r\n";
//----------------------------------------------------------------------------------------------------
static char pattern_CWMODE_Query[] = "AT+CWMODE?";
static char pattern_CWMODE_Cmd[] = "AT+CWMODE=";
static char pattern_CWMODE_Resp[] = "+CWMODE:";

static char pattern_CWMODE_CUR_Query[] = "AT+CWMODE_CUR?";
static char pattern_CWMODE_CUR_Cmd[] = "AT+CWMODE_CUR=";
static char pattern_CWMODE_CUR_Resp[] = "+CWMODE_CUR:";

static char pattern_CWMODE_DEF_Query[] = "AT+CWMODE_DEF?";
static char pattern_CWMODE_DEF_Cmd[] = "AT+CWMODE_DEF=";
static char pattern_CWMODE_DEF_Resp[] = "+CWMODE_DEF:";
//----------------------------------------------------------------------------------------------------
static char pattern_CWJAP_Query[] = "AT+CWJAP?";
static char pattern_CWJAP_Cmd[] = "AT+CWJAP=";
static char pattern_CWJAP_Resp[] = "+CWJAP:";

static char pattern_CWJAP_CUR_Query[] = "AT+CWJAP_CUR?";
static char pattern_CWJAP_CUR_Cmd[] = "AT+CWJAP_CUR=";
static char pattern_CWJAP_CUR_Resp[] = "+CWJAP_CUR:";

static char pattern_CWJAP_DEF_Query[] = "AT+CWJAP_DEF?";
static char pattern_CWJAP_DEF_Cmd[] = "AT+CWJAP_DEF=";
static char pattern_CWJAP_DEF_Resp[] = "+CWJAP_DEF:";
//----------------------------------------------------------------------------------------------------
static char pattern_CWLAP_Query[] = "AT+CWLAP";
static char pattern_CWLAP_Resp[] = "+CWLAP:";
static char pattern_CWLAP_Cmd[] = "AT+CWLAP=";
//----------------------------------------------------------------------------------------------------
static char pattern_CWQAP_Cmd[] = "AT+CWQAP";
//----------------------------------------------------------------------------------------------------
static char pattern_CWSAP_Query[] = "AT+CWSAP?";
static char pattern_CWSAP_Cmd[] = "AT+CWSAP=";
static char pattern_CWSAP_Resp[] = "+CWSAP:\"ESP8266\",\"1234567890\",5,3\r\n";

static char pattern_CWSAP_CUR_Query[] = "AT+CWSAP_CUR?";
static char pattern_CWSAP_CUR_Cmd[] = "AT+CWSAP_CUR=";
static char pattern_CWSAP_CUR_Resp[] = "+CWSAP_CUR:\"ESP8266\",\"1234567890\",5,3\r\n";

static char pattern_CWSAP_DEF_Query[] = "AT+CWSAP_DEF?";
static char pattern_CWSAP_DEF_Cmd[] = "AT+CWSAP_DEF=";
static char pattern_CWSAP_DEF_Resp[] = "+CWSAP_DEF:\"ESP8266\",\"1234567890\",5,3\r\n";
//----------------------------------------------------------------------------------------------------
static char pattern_CWLIF_CUR_Query[] = "AT+CWLIF";
//----------------------------------------------------------------------------------------------------
static char pattern_CWDHCP_Query[] = "AT+CWDHCP?";
static char pattern_CWDHCP_Resp[] = "+CWDHCP:";
static char pattern_CWDHCP_Cmd[] = "AT+CWDHCP=";

static char pattern_CWDHCP_CUR_Query[] = "AT+CWDHCP_CUR?";
static char pattern_CWDHCP_CUR_Resp[] = "+CWDHCP_CUR:";
static char pattern_CWDHCP_CUR_Cmd[] = "AT+CWDHCP_CUR=";

static char pattern_CWDHCP_DEF_Query[] = "AT+CWDHCP_DEF?";
static char pattern_CWDHCP_DEF_Resp[] = "+CWDHCP_DEF:";
static char pattern_CWDHCP_DEF_Cmd[] = "AT+CWDHCP_DEF=";
//----------------------------------------------------------------------------------------------------
static char pattern_CWDHCPS_CUR_Query[] = "AT+CWDHCPS_CUR";
static char pattern_CWDHCPS_CUR_Resp[] = "+CWDHCPS_CUR:";
static char pattern_CWDHCPS_CUR_Cmd[] = "AT+CWDHCPS_CUR=";
//----------------------------------------------------------------------------------------------------
static char pattern_CWAUTOCONN_Cmd[] = "AT+CWAUTOCONN=";
//----------------------------------------------------------------------------------------------------
static char pattern_CIPSTAMAC_Query[] = "AT+CIPSTAMAC?";
static char pattern_CIPSTAMAC_Resp[] = "+CIPSTAMAC:";
static char pattern_CIPSTAMAC_Cmd[] = "AT+CIPSTAMAC=";

static char pattern_CIPSTAMAC_CUR_Query[] = "AT+CIPSTAMAC_CUR?";
static char pattern_CIPSTAMAC_CUR_Resp[] = "+CIPSTAMAC_CUR:";
static char pattern_CIPSTAMAC_CUR_Cmd[] = "AT+CIPSTAMAC_CUR=";

static char pattern_CIPSTAMAC_DEF_Query[] = "AT+CIPSTAMAC_DEF?";
static char pattern_CIPSTAMAC_DEF_Resp[] = "+CIPSTAMAC_DEF:";
static char pattern_CIPSTAMAC_DEF_Cmd[] = "AT+CIPSTAMAC_DEF=";
//----------------------------------------------------------------------------------------------------
static char pattern_CIPAPMAC_Query[] = "AT+CIPAPMAC?";
static char pattern_CIPAPMAC_Resp[] = "+CIPAPMAC:";
static char pattern_CIPAPMAC_Cmd[] = "AT+CIPAPMAC=";

static char pattern_CIPAPMAC_CUR_Query[] = "AT+CIPAPMAC_CUR?";
static char pattern_CIPAPMAC_CUR_Resp[] = "+CIPAPMAC_CUR:";
static char pattern_CIPAPMAC_CUR_Cmd[] = "AT+CIPAPMAC_CUR=";

static char pattern_CIPAPMAC_DEF_Query[] = "AT+CIPAPMAC_DEF?";
static char pattern_CIPAPMAC_DEF_Resp[] = "+CIPAPMAC_DEF:";
static char pattern_CIPAPMAC_DEF_Cmd[] = "AT+CIPAPMAC_DEF=";
//----------------------------------------------------------------------------------------------------
static char pattern_CIPSTA_Query[] = "AT+CIPSTA?";
static char pattern_CIPSTA_Cmd[] = "AT+CIPSTA=";
static char pattern_CIPSTA_Resp[] = "+CIPSTA:";

static char pattern_CIPSTA_CUR_Query[] = "AT+CIPSTA_CUR?";
static char pattern_CIPSTA_CUR_Cmd[] = "AT+CIPSTA_CUR=";
static char pattern_CIPSTA_CUR_Resp[] = "+CIPSTA_CUR:";

static char pattern_CIPSTA_DEF_Query[] = "AT+CIPSTA_DEF?";
static char pattern_CIPSTA_DEF_Cmd[] = "AT+CIPSTA_DEF=";
static char pattern_CIPSTA_DEF_Resp[] = "+CIPSTA_DEF:";
//----------------------------------------------------------------------------------------------------
static char pattern_CIPAP_Query[] = "AT+CIPAP?";
static char pattern_CIPAP_Resp[] = "+CIPAP:";
static char pattern_CIPAP_Cmd[] = "AT+CIPAP=";

static char pattern_CIPAP_CUR_Query[] = "AT+CIPAP_CUR?";
static char pattern_CIPAP_CUR_Resp[] = "+CIPAP_CUR:";
static char pattern_CIPAP_CUR_Cmd[] = "AT+CIPAP_CUR=";

static char pattern_CIPAP_DEF_Query[] = "AT+CIPAP_DEF?";
static char pattern_CIPAP_DEF_Resp[] = "+CIPAP_DEF:";
static char pattern_CIPAP_DEF_Cmd[] = "AT+CIPAP_DEF=";
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I С  F U N C T I O N   D E F I N I T I O N   __________________
static void transmit_ok(void)
{
//    Serial1.print(pattern_OK);
//    Serial.print(pattern_OK);
}

static void transmit_error(void)
{
//    Serial1.print(pattern_ERROR);
//    Serial.print(pattern_ERROR);
}

static bool cwmode_cur_setup(char msg[])
{
	struct slre_cap caps[1];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWMODE_CUR=(\\d+)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	memcpy((void*)wifiMode, (void*)caps[0].ptr, caps[0].len);
	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cwmode_cur_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, pattern_CWMODE_CUR_Resp, strlen(pattern_CWMODE_CUR_Resp));
	memcpy((void*)(txBuffer + strlen(pattern_CWMODE_CUR_Resp)), wifiMode, strlen(wifiMode));
	nbm = strlen(pattern_CWMODE_CUR_Resp) + strlen(wifiMode);
	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

    return true;
}

static bool cwmode_def_setup(char msg[])
{
	struct slre_cap caps[1];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWMODE_DEF=(\\d+)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	memcpy((void*)wifiMode, (void*)caps[0].ptr, caps[0].len);
	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cwmode_def_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, pattern_CWMODE_DEF_Resp, strlen(pattern_CWMODE_DEF_Resp));
	memcpy((void*)(txBuffer + strlen(pattern_CWMODE_DEF_Resp)), wifiMode, strlen(wifiMode));
	nbm = strlen(pattern_CWMODE_DEF_Resp) + strlen(wifiMode);
	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

    return true;
}

static bool cwjap_cur_setup(char msg[])
{
	static uint8_t state = 0;
	struct slre_cap caps[2];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWJAP_CUR=\"(\\S+)\",\"(\\S+)\"\r\n", msg, strlen(msg), caps, 2, 0) <= 0) {
	   return false;
	}

	switch(state)
	{
		case 0:
			strcpy(txBuffer, "\r\n+CWJAP_CUR:1\r\nFAIL\r\n");
			state++;
			break;
		case 1:
			strcpy(txBuffer, "\r\n+CWJAP_CUR:2\r\nFAIL\r\n");
			state++;
			break;
		case 2:
			strcpy(txBuffer, "\r\n+CWJAP_CUR:3\r\nFAIL\r\n");
			state++;
			break;
		case 3:
			strcpy(txBuffer, "\r\n+CWJAP_CUR:4\r\nFAIL\r\n");
			state++;
			break;
		case 4:
			memcpy((void*)AP_sssid, (void*)caps[0].ptr, caps[0].len);
			memcpy((void*)AP_pwd, (void*)caps[1].ptr, caps[1].len);
			strcpy(txBuffer, "\r\nWIFI DISCONNECT\r\n\r\nWIFI CONNECTED\r\n\r\nWIFI GOT IP\r\n\r\n\r\nOK\r\n");
			state = 0;
			transmit_ok();
			break;
	}

    return true;
}

static bool cwjap_cur_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, pattern_CWJAP_CUR_Resp, strlen(pattern_CWJAP_CUR_Resp));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + strlen(pattern_CWJAP_CUR_Resp) + 1), AP_sssid, strlen(AP_sssid));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + (nbm + 1)), AP_bssid, strlen(AP_bssid));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), AP_channel, strlen(AP_channel));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), AP_rssi, strlen(AP_rssi));
	nbm = strlen(txBuffer);
	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

    return true;
}

static bool cwjap_def_setup(char msg[])
{
	static uint8_t state = 0;
	struct slre_cap caps[2];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWJAP_DEF=\"(\\S+)\",\"(\\S+)\"\r\n", msg, strlen(msg), caps, 2, 0) <= 0) {
	   return false;
	}

	switch(state)
	{
		case 0:
			strcpy(txBuffer, "\r\n+CWJAP_CUR:1\r\nFAIL\r\n");
			state++;
			break;
		case 1:
			strcpy(txBuffer, "\r\n+CWJAP_CUR:2\r\nFAIL\r\n");
			state++;
			break;
		case 2:
			strcpy(txBuffer, "\r\n+CWJAP_CUR:3\r\nFAIL\r\n");
			state++;
			break;
		case 3:
			strcpy(txBuffer, "\r\n+CWJAP_CUR:4\r\nFAIL\r\n");
			state++;
			break;
		case 4:
			memcpy((void*)AP_sssid, (void*)caps[0].ptr, caps[0].len);
			memcpy((void*)AP_pwd, (void*)caps[1].ptr, caps[1].len);
			strcpy(txBuffer, "\r\nWIFI DISCONNECT\r\n\r\nWIFI CONNECTED\r\n\r\nWIFI GOT IP\r\n\r\n\r\nOK\r\n");
			state = 0;
			transmit_ok();
			break;
	}

    return true;
}

static bool cwjap_def_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, pattern_CWJAP_DEF_Resp, strlen(pattern_CWJAP_DEF_Resp));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + strlen(pattern_CWJAP_DEF_Resp) + 1), AP_sssid, strlen(AP_sssid));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + (nbm + 1)), AP_bssid, strlen(AP_bssid));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), AP_channel, strlen(AP_channel));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), AP_rssi, strlen(AP_rssi));
	nbm = strlen(txBuffer);
	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

    return true;
}

static bool cwqap_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, "\r\nOK\r\nWIFI DISCONNECT\r\n");
    return true;
}

static bool cwsap_cur_setup(char msg[])
{
	struct slre_cap caps[4];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWSAP_CUR=\"(\\S+)\",\"(\\S+)\",(\\d+),(\\d+)\r\n", msg, strlen(msg), caps, 4, 0) <= 0) {
	   return false;
	}

	memcpy((void*)SoftAP_ssid, (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)SoftAP_pwd, (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)SoftAP_channel, (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)SoftAP_enc, (void*)caps[3].ptr, caps[3].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cwsap_cur_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, "+CWSAP_CUR:", strlen("+CWSAP_CUR:"));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + strlen("+CWSAP_CUR:") + 1), SoftAP_ssid, strlen(SoftAP_ssid));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + (nbm + 1)), SoftAP_pwd, strlen(SoftAP_pwd));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), SoftAP_channel, strlen(SoftAP_channel));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), SoftAP_enc, strlen(SoftAP_enc));
	nbm = strlen(txBuffer);
	memcpy((void*)(txBuffer + (nbm)), ",1,1\r\n\r\nOK\r\n", strlen(",1,1\r\n\r\nOK\r\n"));

    return true;






//	uint32_t nbm = 0;
//
//	memset(txBuffer,0,sizeof(txBuffer));
//
//	memcpy((void*)txBuffer, "+CWSAP_CUR:", strlen("+CWSAP_CUR:"));
//	memcpy((void*)(txBuffer + strlen("+CWSAP_CUR:")), SoftAP_ssid, strlen(SoftAP_ssid));
//	nbm = strlen("+CWSAP_CUR:") + strlen(SoftAP_ssid);
//	txBuffer[nbm] = ',';
//	memcpy((void*)(txBuffer + (nbm + 1)), SoftAP_pwd, strlen(SoftAP_pwd));
//	txBuffer[nbm + 2] = ',';
//	memcpy((void*)(txBuffer + (nbm + 3)), SoftAP_channel, strlen(SoftAP_channel));
//	txBuffer[nbm + 4] = ',';
//	memcpy((void*)(txBuffer + (nbm + 5)), SoftAP_enc, strlen(SoftAP_enc));
//	memcpy((void*)(txBuffer + (nbm + 6)), ",4,0\r\n\r\nOK\r\n", strlen(",4,0\r\n\r\nOK\r\n"));
//
//    return true;
}

static bool cwsap_def_setup(char msg[])
{
	struct slre_cap caps[4];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWSAP_DEF=(\\S+)\\S(\\S+)\\S(\\d+)\\S(\\d+)\r\n", msg, strlen(msg), caps, 4, 0) <= 0) {
	   return false;
	}

	memcpy((void*)SoftAP_ssid, (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)SoftAP_pwd, (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)SoftAP_channel, (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)SoftAP_enc, (void*)caps[3].ptr, caps[3].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cwsap_def_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, "+CWSAP_DEF:", strlen("+CWSAP_DEF:"));
	memcpy((void*)(txBuffer + strlen("+CWSAP_DEF:")), SoftAP_ssid, strlen(SoftAP_ssid));
	nbm = strlen("+CWSAP_DEF:") + strlen(SoftAP_ssid);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), SoftAP_pwd, strlen(SoftAP_pwd));
	txBuffer[nbm + 2] = ',';
	memcpy((void*)(txBuffer + (nbm + 3)), SoftAP_channel, strlen(SoftAP_channel));
	txBuffer[nbm + 4] = ',';
	memcpy((void*)(txBuffer + (nbm + 5)), SoftAP_enc, strlen(SoftAP_enc));
	memcpy((void*)(txBuffer + (nbm + 6)), ",4,0\r\n\r\nOK\r\n", strlen(",4,0\r\n\r\nOK\r\n"));

    return true;
}

static bool cwlif_request(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, "\"192.168.6.100\",\"ca:d7:19:d8:a6:44\"\r\n\r\nOK\r\n");
    return true;
}

static bool cwdhcp_cur_request(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, "+CWDHCP_CUR:3\r\n\r\nOK\r\n");
    return true;
}

static bool cwdhcp_cur_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool cwdhcp_def_request(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, "+CWDHCP_DEF:3\r\n\r\nOK\r\n");
    return true;
}

static bool cwdhcp_def_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool cwautoconn_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool cipstamac_cur_setup(char msg[])
{
	struct slre_cap caps[6];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPSTAMAC_CUR=\"(\\S+):(\\S+):(\\S+):(\\S+):(\\S+):(\\S+)\"\r\n", msg, strlen(msg), caps, 6, 0) <= 0) {
	   return false;
	}

	memcpy((void*)&Station_mac[0], (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)&Station_mac[3], (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)&Station_mac[6], (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)&Station_mac[9], (void*)caps[3].ptr, caps[3].len);
	memcpy((void*)&Station_mac[12], (void*)caps[4].ptr, caps[4].len);
	memcpy((void*)&Station_mac[15], (void*)caps[5].ptr, caps[5].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cipstamac_cur_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, pattern_CIPSTAMAC_CUR_Resp, strlen(pattern_CIPSTAMAC_CUR_Resp));

	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + nbm + 1), Station_mac, strlen(Station_mac));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + (nbm + 1)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

	return true;
}

static bool cipstamac_def_setup(char msg[])
{
	struct slre_cap caps[6];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPSTAMAC_DEF=\"(\\S+):(\\S+):(\\S+):(\\S+):(\\S+):(\\S+)\"\r\n", msg, strlen(msg), caps, 6, 0) <= 0) {
	   return false;
	}

	memcpy((void*)&Station_mac[0], (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)&Station_mac[3], (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)&Station_mac[6], (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)&Station_mac[9], (void*)caps[3].ptr, caps[3].len);
	memcpy((void*)&Station_mac[12], (void*)caps[4].ptr, caps[4].len);
	memcpy((void*)&Station_mac[15], (void*)caps[5].ptr, caps[5].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cipstamac_def_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, pattern_CIPSTAMAC_DEF_Resp, strlen(pattern_CIPSTAMAC_DEF_Resp));

	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + nbm + 1), Station_mac, strlen(Station_mac));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + (nbm + 1)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

//	memcpy((void*)(txBuffer + strlen(pattern_CIPSTAMAC_DEF_Resp)), Station_mac, strlen(Station_mac));
//	nbm = strlen(txBuffer);
//	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

	return true;
}

static bool cipapmac_cur_setup(char msg[])
{
	struct slre_cap caps[6];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPAPMAC_CUR=\"(\\S+):(\\S+):(\\S+):(\\S+):(\\S+):(\\S+)\"\r\n", msg, strlen(msg), caps, 6, 0) <= 0) {
	   return false;
	}

	memcpy((void*)&SoftAP_mac[0], (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)&SoftAP_mac[3], (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)&SoftAP_mac[6], (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)&SoftAP_mac[9], (void*)caps[3].ptr, caps[3].len);
	memcpy((void*)&SoftAP_mac[12], (void*)caps[4].ptr, caps[4].len);
	memcpy((void*)&SoftAP_mac[15], (void*)caps[5].ptr, caps[5].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cipapmac_cur_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, pattern_CIPSTAMAC_CUR_Resp, strlen(pattern_CIPSTAMAC_CUR_Resp));

	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + nbm + 1), SoftAP_mac, strlen(SoftAP_mac));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + (nbm + 1)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));


//	memcpy((void*)(txBuffer + strlen(pattern_CIPSTAMAC_CUR_Resp)), Station_mac, strlen(Station_mac));
//	nbm = strlen(txBuffer);
//	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

	return true;
}

static bool cipapmac_def_setup(char msg[])
{
	struct slre_cap caps[6];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPAPMAC_DEF=\"(\\S+):(\\S+):(\\S+):(\\S+):(\\S+):(\\S+)\"\r\n", msg, strlen(msg), caps, 6, 0) <= 0) {
	   return false;
	}

	memcpy((void*)&SoftAP_mac[0], (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)&SoftAP_mac[3], (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)&SoftAP_mac[6], (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)&SoftAP_mac[9], (void*)caps[3].ptr, caps[3].len);
	memcpy((void*)&SoftAP_mac[12], (void*)caps[4].ptr, caps[4].len);
	memcpy((void*)&SoftAP_mac[15], (void*)caps[5].ptr, caps[5].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cipapmac_def_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, pattern_CIPSTAMAC_DEF_Resp, strlen(pattern_CIPSTAMAC_DEF_Resp));

	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + nbm + 1), SoftAP_mac, strlen(SoftAP_mac));
	nbm = strlen(txBuffer);
	txBuffer[nbm] = '\"';
	memcpy((void*)(txBuffer + (nbm + 1)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

//	memcpy((void*)(txBuffer + strlen(pattern_CIPSTAMAC_DEF_Resp)), Station_mac, strlen(Station_mac));
//	nbm = strlen(txBuffer);
//	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

	return true;
}

static bool cipsta_cur_setup(char msg[])
{
	struct slre_cap caps[12];
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));
	memset(Station_ip,0,sizeof(Station_ip));
	memset(Station_gw,0,sizeof(Station_gw));
	memset(Station_mask,0,sizeof(Station_mask));

	if (slre_match((const char *)"AT\\+CIPSTA_CUR=\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\"\r\n", msg, strlen(msg), caps, 12, 0) <= 0) {
	   return false;
	}

	memcpy((void*)&Station_ip[0], (void*)caps[0].ptr, caps[0].len);
	nbm = strlen(Station_ip);
	Station_ip[nbm] = '.';
	memcpy((void*)&Station_ip[nbm + 1], (void*)caps[1].ptr, caps[1].len);
	nbm = strlen(Station_ip);
	Station_ip[nbm] = '.';
	memcpy((void*)&Station_ip[nbm + 1], (void*)caps[2].ptr, caps[2].len);
	nbm = strlen(Station_ip);
	Station_ip[nbm] = '.';
	memcpy((void*)&Station_ip[nbm + 1], (void*)caps[3].ptr, caps[3].len);

	memcpy((void*)&Station_gw[0], (void*)caps[4].ptr, caps[4].len);
	nbm = strlen(Station_gw);
	Station_gw[nbm] = '.';
	memcpy((void*)&Station_gw[nbm + 1], (void*)caps[5].ptr, caps[5].len);
	nbm = strlen(Station_gw);
	Station_gw[nbm] = '.';
	memcpy((void*)&Station_gw[nbm + 1], (void*)caps[6].ptr, caps[6].len);
	nbm = strlen(Station_gw);
	Station_gw[nbm] = '.';
	memcpy((void*)&Station_gw[nbm + 1], (void*)caps[7].ptr, caps[7].len);

	memcpy((void*)&Station_mask[0], (void*)caps[8].ptr, caps[8].len);
	nbm = strlen(Station_mask);
	Station_mask[nbm] = '.';
	memcpy((void*)&Station_mask[nbm + 1], (void*)caps[9].ptr, caps[9].len);
	nbm = strlen(Station_mask);
	Station_mask[nbm] = '.';
	memcpy((void*)&Station_mask[nbm + 1], (void*)caps[10].ptr, caps[10].len);
	nbm = strlen(Station_mask);
	Station_mask[nbm] = '.';
	memcpy((void*)&Station_mask[nbm + 1], (void*)caps[11].ptr, caps[11].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool cipsta_cur_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, "+CIPSTA_CUR:\"", strlen("+CIPSTA_CUR:\""));
	memcpy((void*)(txBuffer + strlen("+CIPSTA_CUR:\"")), Station_ip, strlen(Station_ip));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + nbm), "\"\r\n+CIPSTA_CUR:\"", strlen("\"\r\n+CIPSTA_CUR:\""));
	memcpy((void*)(txBuffer + nbm + strlen("\"\r\n+CIPSTA_CUR:\"")), Station_gw, strlen(Station_gw));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + nbm), "\"\r\n+CIPSTA_CUR:\"", strlen("\"\r\n+CIPSTA_CUR:\""));
	memcpy((void*)(txBuffer + nbm + strlen("\"\r\n+CIPSTA_CUR:\"")), Station_mask, strlen(Station_mask));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + (nbm)), "\"\r\n\r\nOK\r\n", strlen("\"\r\n\r\nOK\r\n"));

	return true;
}

static bool cipsta_def_setup(char msg[])
{
	struct slre_cap caps[12];
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));
	memset(Station_ip,0,sizeof(Station_ip));
	memset(Station_gw,0,sizeof(Station_gw));
	memset(Station_mask,0,sizeof(Station_mask));

	if (slre_match((const char *)"AT\\+CIPSTA_DEF=\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\"\r\n", msg, strlen(msg), caps, 12, 0) <= 0) {
	   return false;
	}

	memcpy((void*)&Station_ip[0], (void*)caps[0].ptr, caps[0].len);
	nbm = strlen(Station_ip);
	Station_ip[nbm] = '.';
	memcpy((void*)&Station_ip[nbm + 1], (void*)caps[1].ptr, caps[1].len);
	nbm = strlen(Station_ip);
	Station_ip[nbm] = '.';
	memcpy((void*)&Station_ip[nbm + 1], (void*)caps[2].ptr, caps[2].len);
	nbm = strlen(Station_ip);
	Station_ip[nbm] = '.';
	memcpy((void*)&Station_ip[nbm + 1], (void*)caps[3].ptr, caps[3].len);

	memcpy((void*)&Station_gw[0], (void*)caps[4].ptr, caps[4].len);
	nbm = strlen(Station_gw);
	Station_gw[nbm] = '.';
	memcpy((void*)&Station_gw[nbm + 1], (void*)caps[5].ptr, caps[5].len);
	nbm = strlen(Station_gw);
	Station_gw[nbm] = '.';
	memcpy((void*)&Station_gw[nbm + 1], (void*)caps[6].ptr, caps[6].len);
	nbm = strlen(Station_gw);
	Station_gw[nbm] = '.';
	memcpy((void*)&Station_gw[nbm + 1], (void*)caps[7].ptr, caps[7].len);

	memcpy((void*)&Station_mask[0], (void*)caps[8].ptr, caps[8].len);
	nbm = strlen(Station_mask);
	Station_mask[nbm] = '.';
	memcpy((void*)&Station_mask[nbm + 1], (void*)caps[9].ptr, caps[9].len);
	nbm = strlen(Station_mask);
	Station_mask[nbm] = '.';
	memcpy((void*)&Station_mask[nbm + 1], (void*)caps[10].ptr, caps[10].len);
	nbm = strlen(Station_mask);
	Station_mask[nbm] = '.';
	memcpy((void*)&Station_mask[nbm + 1], (void*)caps[11].ptr, caps[11].len);

	strcpy(txBuffer, pattern_OK1);

	return true;
}

static bool cipsta_def_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, "+CIPSTA_DEF:\"", strlen("+CIPSTA_DEF:\""));
	memcpy((void*)(txBuffer + strlen("+CIPSTA_DEF:\"")), Station_ip, strlen(Station_ip));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + nbm), "\"\r\n+CIPSTA_DEF:\"", strlen("\"\r\n+CIPSTA_DEF:\""));
	memcpy((void*)(txBuffer + nbm + strlen("\"\r\n+CIPSTA_DEF:\"")), Station_gw, strlen(Station_gw));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + nbm), "\"\r\n+CIPSTA_DEF:\"", strlen("\"\r\n+CIPSTA_DEF:\""));
	memcpy((void*)(txBuffer + nbm + strlen("\"\r\n+CIPSTA_DEF:\"")), Station_mask, strlen(Station_mask));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + (nbm)), "\"\r\n\r\nOK\r\n", strlen("\"\r\n\r\nOK\r\n"));

	return true;
}

static bool cipap_cur_setup(char msg[])
{
	struct slre_cap caps[12];
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));
	memset(SoftAP_ip,0,sizeof(SoftAP_ip));
	memset(SoftAP_gw,0,sizeof(SoftAP_gw));
	memset(SoftAP_mask,0,sizeof(SoftAP_mask));

	if (slre_match((const char *)"AT\\+CIPAP_CUR=\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\"\r\n", msg, strlen(msg), caps, 12, 0) <= 0) {
	   return false;
	}

	memcpy((void*)&SoftAP_ip[0], (void*)caps[0].ptr, caps[0].len);
	nbm = strlen(SoftAP_ip);
	SoftAP_ip[nbm] = '.';
	memcpy((void*)&SoftAP_ip[nbm + 1], (void*)caps[1].ptr, caps[1].len);
	nbm = strlen(SoftAP_ip);
	SoftAP_ip[nbm] = '.';
	memcpy((void*)&SoftAP_ip[nbm + 1], (void*)caps[2].ptr, caps[2].len);
	nbm = strlen(SoftAP_ip);
	SoftAP_ip[nbm] = '.';
	memcpy((void*)&SoftAP_ip[nbm + 1], (void*)caps[3].ptr, caps[3].len);

	memcpy((void*)&SoftAP_gw[0], (void*)caps[4].ptr, caps[4].len);
	nbm = strlen(SoftAP_gw);
	SoftAP_gw[nbm] = '.';
	memcpy((void*)&SoftAP_gw[nbm + 1], (void*)caps[5].ptr, caps[5].len);
	nbm = strlen(SoftAP_gw);
	SoftAP_gw[nbm] = '.';
	memcpy((void*)&SoftAP_gw[nbm + 1], (void*)caps[6].ptr, caps[6].len);
	nbm = strlen(SoftAP_gw);
	SoftAP_gw[nbm] = '.';
	memcpy((void*)&SoftAP_gw[nbm + 1], (void*)caps[7].ptr, caps[7].len);

	memcpy((void*)&SoftAP_mask[0], (void*)caps[8].ptr, caps[8].len);
	nbm = strlen(SoftAP_mask);
	SoftAP_mask[nbm] = '.';
	memcpy((void*)&SoftAP_mask[nbm + 1], (void*)caps[9].ptr, caps[9].len);
	nbm = strlen(SoftAP_mask);
	SoftAP_mask[nbm] = '.';
	memcpy((void*)&SoftAP_mask[nbm + 1], (void*)caps[10].ptr, caps[10].len);
	nbm = strlen(SoftAP_mask);
	SoftAP_mask[nbm] = '.';
	memcpy((void*)&SoftAP_mask[nbm + 1], (void*)caps[11].ptr, caps[11].len);

	strcpy(txBuffer, pattern_OK1);

	return true;
}

static bool cipap_cur_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, "+CIPAP_CUR:\"", strlen("+CIPAP_CUR:\""));
	memcpy((void*)(txBuffer + strlen("+CIPAP_CUR:\"")), Station_ip, strlen(Station_ip));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + nbm), "\"\r\n+CIPAP_CUR:\"", strlen("\"\r\n+CIPAP_CUR:\""));
	memcpy((void*)(txBuffer + nbm + strlen("\"\r\n+CIPAP_CUR:\"")), Station_gw, strlen(Station_gw));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + nbm), "\"\r\n+CIPAP_CUR:\"", strlen("\"\r\n+CIPAP_CUR:\""));
	memcpy((void*)(txBuffer + nbm + strlen("\"\r\n+CIPAP_CUR:\"")), Station_mask, strlen(Station_mask));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + (nbm)), "\"\r\n\r\nOK\r\n", strlen("\"\r\n\r\nOK\r\n"));

	return true;
}

static bool cipap_def_setup(char msg[])
{
	struct slre_cap caps[12];
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));
	memset(SoftAP_ip,0,sizeof(SoftAP_ip));
	memset(SoftAP_gw,0,sizeof(SoftAP_gw));
	memset(SoftAP_mask,0,sizeof(SoftAP_mask));

	if (slre_match((const char *)"AT\\+CIPAP_DEF=\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\"\r\n", msg, strlen(msg), caps, 12, 0) <= 0) {
	   return false;
	}

	memcpy((void*)&SoftAP_ip[0], (void*)caps[0].ptr, caps[0].len);
	nbm = strlen(SoftAP_ip);
	SoftAP_ip[nbm] = '.';
	memcpy((void*)&SoftAP_ip[nbm + 1], (void*)caps[1].ptr, caps[1].len);
	nbm = strlen(SoftAP_ip);
	SoftAP_ip[nbm] = '.';
	memcpy((void*)&SoftAP_ip[nbm + 1], (void*)caps[2].ptr, caps[2].len);
	nbm = strlen(SoftAP_ip);
	SoftAP_ip[nbm] = '.';
	memcpy((void*)&SoftAP_ip[nbm + 1], (void*)caps[3].ptr, caps[3].len);

	memcpy((void*)&SoftAP_gw[0], (void*)caps[4].ptr, caps[4].len);
	nbm = strlen(SoftAP_gw);
	SoftAP_gw[nbm] = '.';
	memcpy((void*)&SoftAP_gw[nbm + 1], (void*)caps[5].ptr, caps[5].len);
	nbm = strlen(SoftAP_gw);
	SoftAP_gw[nbm] = '.';
	memcpy((void*)&SoftAP_gw[nbm + 1], (void*)caps[6].ptr, caps[6].len);
	nbm = strlen(SoftAP_gw);
	SoftAP_gw[nbm] = '.';
	memcpy((void*)&SoftAP_gw[nbm + 1], (void*)caps[7].ptr, caps[7].len);

	memcpy((void*)&SoftAP_mask[0], (void*)caps[8].ptr, caps[8].len);
	nbm = strlen(SoftAP_mask);
	SoftAP_mask[nbm] = '.';
	memcpy((void*)&SoftAP_mask[nbm + 1], (void*)caps[9].ptr, caps[9].len);
	nbm = strlen(SoftAP_mask);
	SoftAP_mask[nbm] = '.';
	memcpy((void*)&SoftAP_mask[nbm + 1], (void*)caps[10].ptr, caps[10].len);
	nbm = strlen(SoftAP_mask);
	SoftAP_mask[nbm] = '.';
	memcpy((void*)&SoftAP_mask[nbm + 1], (void*)caps[11].ptr, caps[11].len);

	strcpy(txBuffer, pattern_OK1);

	return true;
}

static bool cipap_def_request(char msg[])
{
	uint32_t nbm = 0;

	memset(txBuffer,0,sizeof(txBuffer));

	memcpy((void*)txBuffer, "+CIPAP_DEF:\"", strlen("+CIPAP_DEF:\""));
	memcpy((void*)(txBuffer + strlen("+CIPAP_DEF:\"")), Station_ip, strlen(Station_ip));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + nbm), "\"\r\n+CIPAP_DEF:\"", strlen("\"\r\n+CIPAP_DEF:\""));
	memcpy((void*)(txBuffer + nbm + strlen("\"\r\n+CIPAP_DEF:\"")), Station_gw, strlen(Station_gw));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + nbm), "\"\r\n+CIPAP_DEF:\"", strlen("\"\r\n+CIPAP_DEF:\""));
	memcpy((void*)(txBuffer + nbm + strlen("\"\r\n+CIPAP_DEF:\"")), Station_mask, strlen(Station_mask));
	nbm = strlen(txBuffer);

	memcpy((void*)(txBuffer + (nbm)), "\"\r\n\r\nOK\r\n", strlen("\"\r\n\r\nOK\r\n"));

	return true;
}
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
 * @brief	Function handle of all at wi-fi commands.
 * @param   msg[in]	- Received at message.
 * @return  true/false.
**/
bool esp_wifi_handle(char msg[])
{  
	if(strstr(msg, pattern_CWMODE_CUR_Query) != NULL)
	{
		cwmode_cur_request(msg);
        return true;  
	}
	if(strstr(msg, pattern_CWMODE_CUR_Cmd) != NULL)
	{
		cwmode_cur_setup(msg);
        return true;  
	}

    if(strstr(msg, pattern_CWMODE_DEF_Query) != NULL)
    {
        cwmode_def_request(msg);
        return true;  
    }

    if(strstr(msg, pattern_CWMODE_DEF_Cmd) != NULL)
    {
        cwmode_def_setup(msg);
        return true;  
    }

    if(strstr(msg, pattern_CWJAP_CUR_Cmd) != NULL)
    {
        cwjap_cur_setup(msg);
        return true;
    }

    if(strstr(msg, pattern_CWJAP_CUR_Query) != NULL)
    {
        cwjap_cur_request(msg);
        return true;
    }
    if(strstr(msg, pattern_CWJAP_DEF_Cmd) != NULL)
    {
        cwjap_def_setup(msg);
        return true;
    }

    if(strstr(msg, pattern_CWJAP_DEF_Query) != NULL)
    {
        cwjap_def_request(msg);
        return true;
    }
 
    if(strstr(msg, pattern_CWQAP_Cmd) != NULL)
    {
        cwqap_setup(msg);
        return true;  
    }

    if(strstr(msg, pattern_CWSAP_CUR_Query) != NULL)
    {
        cwsap_cur_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CWSAP_CUR_Cmd) != NULL)
    {
        cwsap_cur_setup(msg);
        return true;      
    }

    if(strstr(msg, pattern_CWSAP_DEF_Query) != NULL)
    {
        cwsap_def_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CWSAP_DEF_Cmd) != NULL)
    {
        cwsap_def_setup(msg);
        return true;      
    }

    if(strstr(msg, pattern_CWLIF_CUR_Query) != NULL)
    {
        cwlif_request(msg);
        return true;
    }

    if(strstr(msg, pattern_CWDHCP_CUR_Query) != NULL)
    {
        cwdhcp_cur_request(msg);
        return true;   
    }

    if(strstr(msg, pattern_CWDHCP_CUR_Cmd) != NULL)
    {
        cwdhcp_cur_setup(msg);
        return true;      
    }
    if(strstr(msg, pattern_CWDHCP_DEF_Query) != NULL)
    {
        cwdhcp_def_request(msg);
        return true;   
    }

    if(strstr(msg, pattern_CWDHCP_DEF_Cmd) != NULL)
    {
        cwdhcp_def_setup(msg);
        return true;      
    }

    if(strstr(msg, pattern_CWAUTOCONN_Cmd) != NULL)
    {
        cwautoconn_setup(msg);
        return true;      
    }

    if(strstr(msg, pattern_CIPSTAMAC_CUR_Query) != NULL)
    {
        cipstamac_cur_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPSTAMAC_CUR_Cmd) != NULL)
    {
        cipstamac_cur_setup(msg);
        return true;      
    }

    if(strstr(msg, pattern_CIPSTAMAC_DEF_Query) != NULL)
    {
        cipstamac_def_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPSTAMAC_DEF_Cmd) != NULL)
    {
        cipstamac_def_setup(msg);
        return true;      
    }

    if(strstr(msg, pattern_CIPAPMAC_CUR_Query) != NULL)
    {
        cipapmac_cur_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPAPMAC_CUR_Cmd) != NULL)
    {
        cipapmac_cur_setup(msg);
        return true;      
    }
    if(strstr(msg, pattern_CIPAPMAC_DEF_Query) != NULL)
    {
        cipapmac_def_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPAPMAC_DEF_Cmd) != NULL)
    {
        cipapmac_def_setup(msg);
        return true;      
    }

    if(strstr(msg, pattern_CIPSTA_CUR_Query) != NULL)
    {
        cipsta_cur_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPSTA_CUR_Cmd) != NULL)
    {
        cipsta_cur_setup(msg);
        return true;      
    }
    if(strstr(msg, pattern_CIPSTA_DEF_Query) != NULL)
    {
        cipsta_def_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPSTA_DEF_Cmd) != NULL)
    {
        cipsta_def_setup(msg);
        return true;      
    }

    if(strstr(msg, pattern_CIPAP_CUR_Query) != NULL)
    {
        cipap_cur_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPAP_CUR_Cmd) != NULL)
    {
        cipap_cur_setup(msg);
        return true;      
    }
    if(strstr(msg, pattern_CIPAP_DEF_Query) != NULL)
    {
        cipap_def_request(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPAP_DEF_Cmd) != NULL)
    {
        cipap_def_setup(msg);
        return true;      
    }

//    transmit_error();
    return false;
}
