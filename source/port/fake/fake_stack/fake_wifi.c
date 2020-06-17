/********************************************************************************
* @author  		Kovalchuk Alexander
* @email  		alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
* 				which use for...
********************************************************************************/
#if defined(UNIT_TESTING)
//_____ I N C L U D E S _______________________________________________________
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "slre.h"
//_____ C O N F I G S  ________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
#define  BUFFER_LENGTH             200u
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
extern char txBuffer[BUFFER_LENGTH];

static char AP_sssid[32] = "ESPTestAtCommand";

static char AP_pwd[64] = "1234587";
static char AP_bssid[] = "ca:d7:19:d8:a6:44";
static char AP_channel[] = "1";
static char AP_rssi[] = "-125";

static char SoftAP_ssid[32] = "ESPTestAtCommand";
static char SoftAP_pwd[64] = "123456789ABCDEF";
static char SoftAP_channel[] = "1";
static char SoftAP_enc[] = "0";

static char Station_mac[] = "13:fa:30:87:d3:2b";
static char SoftAP_mac[] = "13:fa:30:87:d3:2b";

static char Station_ip[] = "192.168.6.100\0";
static char Station_gw[] = "192.168.6.1\0";
static char Station_mask[] = "255.255.255.0\0";

static char pattern_OK[] = "\r\nOK\r\n";

static char pattern_CWMODE_CUR_Query[] = "AT+CWMODE_CUR?";
static char pattern_CWMODE_CUR_Cmd[] = "AT+CWMODE_CUR=";
static char pattern_CWMODE_CUR_Resp[] = "+CWMODE_CUR:";

static char pattern_CWMODE_DEF_Query[] = "AT+CWMODE_DEF?";
static char pattern_CWMODE_DEF_Cmd[] = "AT+CWMODE_DEF=";
static char pattern_CWMODE_DEF_Resp[] = "+CWMODE_DEF:";

static char pattern_CWJAP_CUR_Query[] = "AT+CWJAP_CUR?";
static char pattern_CWJAP_CUR_Cmd[] = "AT+CWJAP_CUR=";
static char pattern_CWJAP_CUR_Resp[] = "+CWJAP_CUR:";

static char pattern_CWJAP_DEF_Query[] = "AT+CWJAP_DEF?";
static char pattern_CWJAP_DEF_Cmd[] = "AT+CWJAP_DEF=";
static char pattern_CWJAP_DEF_Resp[] = "+CWJAP_DEF:";

static char pattern_CWSAP_CUR_Query[] = "AT+CWSAP_CUR?";
static char pattern_CWSAP_CUR_Cmd[] = "AT+CWSAP_CUR=";

static char pattern_CWQAP_Cmd[] = "AT+CWQAP";

static char pattern_CWSAP_DEF_Query[] = "AT+CWSAP_DEF?";
static char pattern_CWSAP_DEF_Cmd[] = "AT+CWSAP_DEF=";

static char pattern_CWDHCP_CUR_Cmd[] = "AT+CWDHCP_CUR=";
static char pattern_CWDHCP_DEF_Cmd[] = "AT+CWDHCP_DEF=";

static char pattern_CWAUTOCONN_Cmd[] = "AT+CWAUTOCONN=";

static char pattern_CIPSTAMAC_CUR_Query[] = "AT+CIPSTAMAC_CUR?";
static char pattern_CIPSTAMAC_CUR_Resp[] = "+CIPSTAMAC_CUR:";
static char pattern_CIPSTAMAC_CUR_Cmd[] = "AT+CIPSTAMAC_CUR=";

static char pattern_CIPSTAMAC_DEF_Query[] = "AT+CIPSTAMAC_DEF?";
static char pattern_CIPSTAMAC_DEF_Resp[] = "+CIPSTAMAC_DEF:";
static char pattern_CIPSTAMAC_DEF_Cmd[] = "AT+CIPSTAMAC_DEF=";

static char pattern_CIPAPMAC_CUR_Query[] = "AT+CIPAPMAC_CUR?";
static char pattern_CIPAPMAC_CUR_Cmd[] = "AT+CIPAPMAC_CUR=";

static char pattern_CIPAPMAC_DEF_Query[] = "AT+CIPAPMAC_DEF?";
static char pattern_CIPAPMAC_DEF_Cmd[] = "AT+CIPAPMAC_DEF=";

static char pattern_CIPSTA_CUR_Query[] = "AT+CIPSTA_CUR?";
static char pattern_CIPSTA_CUR_Cmd[] = "AT+CIPSTA_CUR=";

static char pattern_CIPSTA_DEF_Query[] = "AT+CIPSTA_DEF?";
static char pattern_CIPSTA_DEF_Cmd[] = "AT+CIPSTA_DEF=";

static char pattern_CIPAP_CUR_Query[] = "AT+CIPAP_CUR?";

static char pattern_CIPAP_CUR_Cmd[] = "AT+CIPAP_CUR=";

static char pattern_CIPAP_DEF_Query[] = "AT+CIPAP_DEF?";
static char pattern_CIPAP_DEF_Cmd[] = "AT+CIPAP_DEF=";

#if 0
static char pattern_CWDHCP_DEF_Query[] = "AT+CWDHCP_DEF?";
static char pattern_CWDHCP_DEF_Resp[] = "+CWDHCP_DEF:";

static char pattern_CWLIF_CUR_Query[] = "AT+CWLIF";

static char pattern_CWDHCP_Query[] = "AT+CWDHCP?";
static char pattern_CWDHCP_Resp[] = "+CWDHCP:";
static char pattern_CWDHCP_Cmd[] = "AT+CWDHCP=";

static char pattern_CWDHCP_CUR_Query[] = "AT+CWDHCP_CUR?";
static char pattern_CWDHCP_CUR_Resp[] = "+CWDHCP_CUR:";

static char pattern_CWMODE_Query[] = "AT+CWMODE?";
static char pattern_CWMODE_Cmd[] = "AT+CWMODE=";
static char pattern_CWMODE_Resp[] = "+CWMODE:";

static char pattern_CWJAP_Query[] = "AT+CWJAP?";
static char pattern_CWJAP_Cmd[] = "AT+CWJAP=";
static char pattern_CWJAP_Resp[] = "+CWJAP:";

static char pattern_CWLAP_Query[] = "AT+CWLAP";
static char pattern_CWLAP_Resp[] = "+CWLAP:";
static char pattern_CWLAP_Cmd[] = "AT+CWLAP=";

static char pattern_CWSAP_Query[] = "AT+CWSAP?";
static char pattern_CWSAP_Cmd[] = "AT+CWSAP=";
static char pattern_CWSAP_Resp[] = "+CWSAP:\"ESP8266\",\"1234567890\",5,3\r\n";
static char pattern_CWSAP_CUR_Resp[] = "+CWSAP_CUR:\"ESP8266\",\"1234567890\",5,3\r\n";
static char pattern_CWSAP_DEF_Resp[] = "+CWSAP_DEF:\"ESP8266\",\"1234567890\",5,3\r\n";

static char pattern_CWDHCPS_CUR_Query[] = "AT+CWDHCPS_CUR";
static char pattern_CWDHCPS_CUR_Resp[] = "+CWDHCPS_CUR:";
static char pattern_CWDHCPS_CUR_Cmd[] = "AT+CWDHCPS_CUR=";

static char pattern_CIPSTAMAC_Query[] = "AT+CIPSTAMAC?";
static char pattern_CIPSTAMAC_Resp[] = "+CIPSTAMAC:";
static char pattern_CIPSTAMAC_Cmd[] = "AT+CIPSTAMAC=";

static char pattern_CIPAPMAC_Query[] = "AT+CIPAPMAC?";
static char pattern_CIPAPMAC_Resp[] = "+CIPAPMAC:";
static char pattern_CIPAPMAC_Cmd[] = "AT+CIPAPMAC=";

static char pattern_CIPAPMAC_CUR_Resp[] = "+CIPAPMAC_CUR:";

static char pattern_CIPAPMAC_DEF_Resp[] = "+CIPAPMAC_DEF:";

static char pattern_CIPSTA_Query[] = "AT+CIPSTA?";
static char pattern_CIPSTA_Cmd[] = "AT+CIPSTA=";
static char pattern_CIPSTA_Resp[] = "+CIPSTA:";

static char pattern_CIPSTA_CUR_Resp[] = "+CIPSTA_CUR:";

static char pattern_CIPSTA_DEF_Resp[] = "+CIPSTA_DEF:";

static char pattern_CIPAP_Query[] = "AT+CIPAP?";
static char pattern_CIPAP_Resp[] = "+CIPAP:";
static char pattern_CIPAP_Cmd[] = "AT+CIPAP=";

static char pattern_CIPAP_CUR_Resp[] = "+CIPAP_CUR:";

static char pattern_CIPAP_DEF_Resp[] = "+CIPAP_DEF:";
#endif
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I С  F U N C T I O N   D E F I N I T I O N   __________________
static bool fake_esp_wifi_mode_setup_cur(const char msg[])
{
	struct slre_cap caps[1];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWMODE_CUR=(\\d+)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_wifi_mode_request_cur(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	memcpy((void*)txBuffer, pattern_CWMODE_CUR_Resp, strlen(pattern_CWMODE_CUR_Resp));
	memcpy((void*)(txBuffer + strlen(pattern_CWMODE_CUR_Resp)), "1", strlen("1"));
	uint32_t nbm = strlen(pattern_CWMODE_CUR_Resp) + strlen("1");
	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));
    return true;
}

static bool fake_esp_wifi_mode_setup_def(const char msg[])
{
	struct slre_cap caps[1];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWMODE_DEF=(\\d+)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_wifi_mode_request_def(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	memcpy((void*)txBuffer, pattern_CWMODE_DEF_Resp, strlen(pattern_CWMODE_DEF_Resp));
	memcpy((void*)(txBuffer + strlen(pattern_CWMODE_DEF_Resp)), "1", strlen("1"));
	uint32_t nbm = strlen(pattern_CWMODE_DEF_Resp) + strlen("1");
	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));
    return true;
}

static bool fake_esp_wifi_ap_join_cur(const char msg[])
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
			memcpy((void*)AP_sssid, (void*)caps[0].ptr, caps[0].len);
			memcpy((void*)AP_pwd, (void*)caps[1].ptr, caps[1].len);
			strcpy(txBuffer, "\r\nWIFI DISCONNECT\r\n\r\nWIFI CONNECTED\r\n\r\nWIFI GOT IP\r\n\r\n\r\nOK\r\n");
			state = 0;
			break;
	}

    return true;
}

static bool fake_esp_wifi_ap_ssid_request_cur(const char msg[])
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

static bool fake_esp_wifi_ap_join_def(const char msg[])
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
			strcpy(txBuffer, "\r\n+CWJAP_DEF:1\r\nFAIL\r\n");
			state++;
			break;
		case 1:
			memcpy((void*)AP_sssid, (void*)caps[0].ptr, caps[0].len);
			memcpy((void*)AP_pwd, (void*)caps[1].ptr, caps[1].len);
			strcpy(txBuffer, "\r\nWIFI DISCONNECT\r\n\r\nWIFI CONNECTED\r\n\r\nWIFI GOT IP\r\n\r\n\r\nOK\r\n");
			state = 0;
			break;
	}

    return true;
}

static bool fake_esp_wifi_ap_ssid_request_def(const char msg[])
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

static bool fake_esp_wifi_ap_unjoin(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, "\r\nOK\r\nWIFI DISCONNECT\r\n");
    return true;
}

static bool fake_esp_wifi_softap_setup_cur(const char msg[])
{
	struct slre_cap caps[4];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWSAP_CUR=\"(\\S+)\",\"(\\S+)\",(\\d+),(\\d+)\r\n", msg, strlen(msg), caps, 4, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_wifi_softap_request_cur(const char msg[])
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
}

static bool fake_esp_wifi_softap_setup_def(const char msg[])
{
	struct slre_cap caps[4];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CWSAP_DEF=(\\S+)\\S(\\S+)\\S(\\d+)\\S(\\d+)\r\n", msg, strlen(msg), caps, 4, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_wifi_softap_request_def(const char msg[])
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

static bool fake_esp_dhcp_mode_setup_cur(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_dhcp_mode_setup_def(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_wifi_autoconnect(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_wifi_station_ip_setup_cur(const char msg[])
{
	struct slre_cap caps[12];

	if (slre_match((const char *)"AT\\+CIPSTA_CUR=\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\"\r\n", msg, strlen(msg), caps, 12, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);

    return true;
}

static bool fake_esp_wifi_station_ip_request_cur(const char msg[])
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

static bool fake_esp_wifi_station_ip_setup_def(const char msg[])
{
	struct slre_cap caps[12];

	if (slre_match((const char *)"AT\\+CIPSTA_DEF=\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\"\r\n", msg, strlen(msg), caps, 12, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);

	return true;
}

static bool fake_esp_wifi_station_ip_request_def(const char msg[])
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

static bool fake_esp_wifi_softap_ip_setup_cur(const char msg[])
{
	struct slre_cap caps[12];

	if (slre_match((const char *)"AT\\+CIPAP_CUR=\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\"\r\n", msg, strlen(msg), caps, 12, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);

	return true;
}

static bool fake_esp_wifi_softap_ip_request_cur(const char msg[])
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

static bool fake_esp_wifi_softap_ip_setup_def(const char msg[])
{
	struct slre_cap caps[12];

	if (slre_match((const char *)"AT\\+CIPAP_DEF=\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\",\"(\\d+).(\\d+).(\\d+).(\\d+)\"\r\n", msg, strlen(msg), caps, 12, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);

	return true;
}

static bool fake_esp_wifi_softap_ip_request_def(const char msg[])
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

static bool fake_esp_wifi_station_mac_setup_cur(const char msg[])
{
	struct slre_cap caps[6];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPSTAMAC_CUR=\"(\\S+):(\\S+):(\\S+):(\\S+):(\\S+):(\\S+)\"\r\n", msg, strlen(msg), caps, 6, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);

    return true;
}

static bool fake_esp_wifi_station_mac_request_cur(const char msg[])
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

static bool fake_esp_wifi_station_mac_setup_def(const char msg[])
{
	struct slre_cap caps[6];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPSTAMAC_DEF=\"(\\S+):(\\S+):(\\S+):(\\S+):(\\S+):(\\S+)\"\r\n", msg, strlen(msg), caps, 6, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);

    return true;
}

static bool fake_esp_wifi_station_mac_request_def(const char msg[])
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

	return true;
}

static bool fake_esp_wifi_softap_mac_setup_cur(const char msg[])
{
	struct slre_cap caps[6];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPAPMAC_CUR=\"(\\S+):(\\S+):(\\S+):(\\S+):(\\S+):(\\S+)\"\r\n", msg, strlen(msg), caps, 6, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);

    return true;
}

static bool fake_esp_wifi_softap_mac_request_cur(const char msg[])
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

	return true;
}

static bool fake_esp_wifi_softap_mac_setup_def(const char msg[])
{
	struct slre_cap caps[6];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPAPMAC_DEF=\"(\\S+):(\\S+):(\\S+):(\\S+):(\\S+):(\\S+)\"\r\n", msg, strlen(msg), caps, 6, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);

    return true;
}

static bool fake_esp_wifi_softap_mac_request_def(const char msg[])
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

	return true;
}

#if 0
static bool cwlif_request(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, "\"192.168.6.100\",\"ca:d7:19:d8:a6:44\"\r\n\r\nOK\r\n");
    return true;
}

static bool cwdhcp_cur_request(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, "+CWDHCP_CUR:3\r\n\r\nOK\r\n");
    return true;
}

static bool cwdhcp_def_request(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));

	strcpy(txBuffer, "+CWDHCP_DEF:3\r\n\r\nOK\r\n");
    return true;
}
#endif

//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
bool fake_esp_wifi_handle(const char msg[])
{  
	if(strstr(msg, pattern_CWMODE_CUR_Query) != NULL)
	{
		fake_esp_wifi_mode_request_cur(msg);
        return true;  
	}
	if(strstr(msg, pattern_CWMODE_CUR_Cmd) != NULL)
	{
		fake_esp_wifi_mode_setup_cur(msg);
        return true;  
	}

    if(strstr(msg, pattern_CWMODE_DEF_Query) != NULL)
    {
    	fake_esp_wifi_mode_request_def(msg);
        return true;  
    }

    if(strstr(msg, pattern_CWMODE_DEF_Cmd) != NULL)
    {
    	fake_esp_wifi_mode_setup_def(msg);
        return true;  
    }

    if(strstr(msg, pattern_CWJAP_CUR_Cmd) != NULL)
    {
    	fake_esp_wifi_ap_join_cur(msg);
        return true;
    }

    if(strstr(msg, pattern_CWJAP_CUR_Query) != NULL)
    {
    	fake_esp_wifi_ap_ssid_request_cur(msg);
        return true;
    }

    if(strstr(msg, pattern_CWJAP_DEF_Cmd) != NULL)
    {
    	fake_esp_wifi_ap_join_def(msg);
        return true;
    }

    if(strstr(msg, pattern_CWJAP_DEF_Query) != NULL)
    {
    	fake_esp_wifi_ap_ssid_request_def(msg);
        return true;
    }

    if(strstr(msg, pattern_CWQAP_Cmd) != NULL)
    {
    	fake_esp_wifi_ap_unjoin(msg);
        return true;  
    }

    if(strstr(msg, pattern_CWSAP_CUR_Query) != NULL)
    {
    	fake_esp_wifi_softap_request_cur(msg);
        return true;   
    }

    if(strstr(msg, pattern_CWSAP_CUR_Cmd) != NULL)
    {
    	fake_esp_wifi_softap_setup_cur(msg);
        return true;      
    }

    if(strstr(msg, pattern_CWSAP_DEF_Query) != NULL)
    {
    	fake_esp_wifi_softap_request_def(msg);
        return true;   
    }

    if(strstr(msg, pattern_CWSAP_DEF_Cmd) != NULL)
    {
    	fake_esp_wifi_softap_setup_def(msg);
        return true;      
    }

    if(strstr(msg, pattern_CWDHCP_CUR_Cmd) != NULL)
    {
    	fake_esp_dhcp_mode_setup_cur(msg);
        return true;
    }

    if(strstr(msg, pattern_CWDHCP_DEF_Cmd) != NULL)
    {
    	fake_esp_dhcp_mode_setup_def(msg);
        return true;
    }

    if(strstr(msg, pattern_CWAUTOCONN_Cmd) != NULL)
    {
    	fake_esp_wifi_autoconnect(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPSTA_CUR_Query) != NULL)
    {
    	fake_esp_wifi_station_ip_request_cur(msg);
        return true;
    }
    if(strstr(msg, pattern_CIPSTA_CUR_Cmd) != NULL)
    {
    	fake_esp_wifi_station_ip_setup_cur(msg);
        return true;
    }
    if(strstr(msg, pattern_CIPSTA_DEF_Query) != NULL)
    {
    	fake_esp_wifi_station_ip_request_def(msg);
        return true;
    }
    if(strstr(msg, pattern_CIPSTA_DEF_Cmd) != NULL)
    {
    	fake_esp_wifi_station_ip_setup_def(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPAP_CUR_Query) != NULL)
    {
    	fake_esp_wifi_softap_ip_request_cur(msg);
        return true;
    }
    if(strstr(msg, pattern_CIPAP_CUR_Cmd) != NULL)
    {
    	fake_esp_wifi_softap_ip_setup_cur(msg);
        return true;
    }
    if(strstr(msg, pattern_CIPAP_DEF_Query) != NULL)
    {
    	fake_esp_wifi_softap_ip_request_def(msg);
        return true;
    }
    if(strstr(msg, pattern_CIPAP_DEF_Cmd) != NULL)
    {
    	fake_esp_wifi_softap_ip_setup_def(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPSTAMAC_CUR_Query) != NULL)
    {
    	fake_esp_wifi_station_mac_request_cur(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPSTAMAC_CUR_Cmd) != NULL)
    {
    	fake_esp_wifi_station_mac_setup_cur(msg);
        return true;      
    }

    if(strstr(msg, pattern_CIPSTAMAC_DEF_Query) != NULL)
    {
    	fake_esp_wifi_station_mac_request_def(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPSTAMAC_DEF_Cmd) != NULL)
    {
    	fake_esp_wifi_station_mac_setup_def(msg);
        return true;      
    }

    if(strstr(msg, pattern_CIPAPMAC_CUR_Query) != NULL)
    {
    	fake_esp_wifi_softap_mac_request_cur(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPAPMAC_CUR_Cmd) != NULL)
    {
    	fake_esp_wifi_softap_mac_setup_cur(msg);
        return true;      
    }
    if(strstr(msg, pattern_CIPAPMAC_DEF_Query) != NULL)
    {
    	fake_esp_wifi_softap_mac_request_def(msg);
        return true;   
    }
    if(strstr(msg, pattern_CIPAPMAC_DEF_Cmd) != NULL)
    {
    	fake_esp_wifi_softap_mac_setup_def(msg);
        return true;      
    }

#if 0
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

    if(strstr(msg, pattern_CWDHCP_DEF_Query) != NULL)
    {
        cwdhcp_def_request(msg);
        return true;
    }

    //    transmit_error();
#endif

    return false;
}
#endif
