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

static char pattern_OK[] = "\r\nOK\r\n";
char pattern_CIPSTATUS_Cmd[] = "AT+CIPSTATUS";
char pattern_CIPSTATUS_Resp[] = "STATUS:2\r\n+CIPSTATUS:0,\"TCP\",\"192.168.100.7\",9786,9988,0\r\n\r\nOK\r\n";
char pattern_CIPSTART_Query[] = "AT+CIPSTART?";
char pattern_CIPSTART_Cmd[] = "AT+CIPSTART=";
char pattern_CIPSTART_Resp[] = "+CIPSTART:";
char pattern_CIPCLOSE_Query[] = "AT+CIPCLOSE?";
char pattern_CIPCLOSE_M_Cmd[] = "AT+CIPCLOSE=";
char pattern_CIPCLOSE_S_Cmd[] = "AT+CIPCLOSE";
char pattern_CIPMUX_Query[] = "AT+CIPMUX?";
char pattern_CIPMUX_Cmd[] = "AT+CIPMUX=1";
char pattern_CIPMUX_S_Cmd[] = "AT+CIPMUX=0";
char pattern_CIPMUX_Resp[] = "+CIPMUX:1\r\n\r\nOK\r\n";
char pattern_CIPMODE_Query[] = "AT+CIPMODE?";
char pattern_CIPMODE_Cmd[] = "AT+CIPMODE=";
char pattern_CIPMODE_Resp[] = "+CIPMODE:0\r\n\r\nOK\r\n";
char pattern_CIPSERVER_Cmd[] = "AT+CIPSERVER=1";
char pattern_CIPSERVER_Close_Cmd[] = "AT+CIPSERVER=0";
char pattern_CIPSERVER_Resp[] = "+CIPSERVER:0\r\n\r\nOK\r\n";
char pattern_CIPSERVERMAXCONN_Query[] = "AT+CIPSERVERMAXCONN?";
char pattern_CIPSERVERMAXCONN_Cmd[] = "AT+CIPSERVERMAXCONN=";
char pattern_CIPSERVERMAXCONN_Resp[] = "+CIPSERVERMAXCONN:4\r\n\r\nOK\r\n";
char pattern_CIPSTO_Query[] = "AT+CIPSTO?";
char pattern_CIPSTO_Cmd[] = "AT+CIPSTO=";
char pattern_CIPSTO_Resp[] = "+CIPSTO:3200\r\n\r\nOK\r\n";
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I С  F U N C T I O N   D E F I N I T I O N   __________________
static bool fake_esp_conn_status_request(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	memcpy((void*)txBuffer, pattern_CIPSTATUS_Resp, strlen(pattern_CIPSTATUS_Resp));
	return true;
}

static bool fake_esp_tcp_connect(const char msg[])
{
	struct slre_cap caps[4];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"\\S+CIPSTART=(\\d),\"(\\S+)\",\"(\\S+)\",(\\d+)\\S+", msg, strlen(msg), caps, 4, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_close_connection(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_close_connection_m(const char msg[])
{
	struct slre_cap caps[1];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"\\S+CIPCLOSE=(\\d)\\S+", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_multiple_connection(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_single_connection(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_mux_cfg_request(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	memcpy((void*)txBuffer, pattern_CIPMUX_Resp, strlen(pattern_CIPMUX_Resp));
	return true;
}

static bool fake_esp_transmit_mode_setup(const char msg[])
{
	struct slre_cap caps[1];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPMODE=(\\d)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_transmit_mode_request(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	memcpy((void*)txBuffer, pattern_CIPMODE_Resp, strlen(pattern_CIPMODE_Resp));
	return true;
}

static bool fake_esp_tcp_server_create(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_tcp_server_delete(const char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_tcp_server_maxconn_setup(const char msg[])
{
	struct slre_cap caps[1];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"AT\\+CIPSERVERMAXCONN=(\\d)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_tcp_server_timeout_setup(const char msg[])
{
	struct slre_cap caps[1];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"\\S+CIPSTO=(\\d)\\S+", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
bool fake_esp_tcp_handle(const char msg[])
{  
    if(strstr(msg, pattern_CIPSTATUS_Cmd))
    {
    	fake_esp_conn_status_request(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPSTART_Cmd))
    {
    	fake_esp_tcp_connect(msg);
        return true;
    }

#if 0
    if(strstr(msg, pattern_CIPSTART_Query))
    {
        fake_esp_udp_transmission(msg);
        return true;
    }
#endif

    if(strstr(msg, pattern_CIPCLOSE_S_Cmd))
    {
    	fake_esp_close_connection(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPCLOSE_M_Cmd))
    {
    	fake_esp_close_connection_m(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPMUX_Cmd))
    {
    	fake_esp_multiple_connection(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPMUX_S_Cmd))
    {
    	fake_esp_single_connection(msg);
        return true;
    }



    if(strstr(msg, pattern_CIPMUX_Query))
    {
        fake_esp_mux_cfg_request(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPMODE_Cmd))
    {
    	fake_esp_transmit_mode_setup(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPMODE_Query))
    {
    	fake_transmit_mode_request(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPSERVER_Cmd))
    {
    	fake_esp_tcp_server_create(msg);
        return true;
    }

	if(strstr(msg, pattern_CIPSERVER_Close_Cmd))
    {
		fake_esp_tcp_server_delete(msg);
        return true;
    }


    if(strstr(msg, pattern_CIPSERVERMAXCONN_Cmd))
    {
    	fake_esp_tcp_server_maxconn_setup(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPSTO_Cmd))
    {
    	fake_esp_tcp_server_timeout_setup(msg);
        return true;
    }

    return false;
}
#endif
