/********************************************************************************
* @author  		Kovalchuk Alexander
* @email  		alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
* 				which use for...
********************************************************************************/
#if defined(UNIT_TESTING)
//_____ I N C L U D E S _______________________________________________________
#include <fake_tcp.h>

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

//static char buffer[BUFFER_LENGTH] = {0};
//static char type[] = "TCP";
//static char linkID[] = "0";
//static char remoteIP[] = "000.000.000.000";
//static char remotePort[] = "9874";
//static char localPortUDP[] = "9974";
//static char modeUDP[] = "0";
//static char keepAlive[] = "1000";
//static TCPIP_Mux_t mux = MUX_SINGLE;
//static char mode = '0';
//----------------------------------------------------------------------------------------------------
static char pattern_OK[] = "\r\nOK\r\n";
static char pattern_ERROR[] = "\r\nERROR\r\n";
//----------------------------------------------------------------------------------------------------
char pattern_CIPSTATUS_Cmd[] = "AT+CIPSTATUS";
char pattern_CIPSTATUS_Resp[] = "STATUS:2\r\n+CIPSTATUS:0,\"TCP\",\"192.168.100.7\",9786,9988,0\r\n\r\nOK\r\n";
//----------------------------------------------------------------------------------------------------
char pattern_CIPSTART_Query[] = "AT+CIPSTART?";
char pattern_CIPSTART_Cmd[] = "AT+CIPSTART=";
char pattern_CIPSTART_Resp[] = "+CIPSTART:";

char pattern_CIPCLOSE_Query[] = "AT+CIPCLOSE?";
char pattern_CIPCLOSE_M_Cmd[] = "AT+CIPCLOSE=";
char pattern_CIPCLOSE_S_Cmd[] = "AT+CIPCLOSE";

//----------------------------------------------------------------------------------------------------
char pattern_CIPMUX_Query[] = "AT+CIPMUX?";
char pattern_CIPMUX_Cmd[] = "AT+CIPMUX=";
char pattern_CIPMUX_Resp[] = "+CIPMUX:1\r\n\r\nOK\r\n";
//----------------------------------------------------------------------------------------------------
char pattern_CIPMODE_Query[] = "AT+CIPMODE?";
char pattern_CIPMODE_Cmd[] = "AT+CIPMODE=";
char pattern_CIPMODE_Resp[] = "+CIPMODE:0\r\n\r\nOK\r\n";


char pattern_CIPSERVER_Cmd[] = "AT+CIPSERVER=";
char pattern_CIPSERVER_Resp[] = "+CIPSERVER:0\r\n\r\nOK\r\n";


char pattern_CIPSERVERMAXCONN_Query[] = "AT+CIPSERVERMAXCONN?";
char pattern_CIPSERVERMAXCONN_Cmd[] = "AT+CIPSERVERMAXCONN=";
char pattern_CIPSERVERMAXCONN_Resp[] = "+CIPSERVERMAXCONN:4\r\n\r\nOK\r\n";

char pattern_CIPSTO_Query[] = "AT+CIPSTO?";
char pattern_CIPSTO_Cmd[] = "AT+CIPSTO=";
char pattern_CIPSTO_Resp[] = "+CIPSTO:3200\r\n\r\nOK\r\n";
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

static bool cipstatus_request(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	memcpy((void*)txBuffer, pattern_CIPSTATUS_Resp, strlen(pattern_CIPSTATUS_Resp));
	return true;
}

static bool cipstart_setup(char msg[])
{
	struct slre_cap caps[4];

	memset(txBuffer,0,sizeof(txBuffer));

	if (slre_match((const char *)"\\S+CIPSTART=(\\d),\"(\\S+)\",\"(\\S+)\",(\\d+)\\S+", msg, strlen(msg), caps, 4, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}


static bool cipclose_s_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}


static bool cipclose_m_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool cipmux_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool cipmux_request(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	memcpy((void*)txBuffer, pattern_CIPMUX_Resp, strlen(pattern_CIPMUX_Resp));
	return true;
}


static bool cipmode_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool cipmode_request(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	memcpy((void*)txBuffer, pattern_CIPMODE_Resp, strlen(pattern_CIPMODE_Resp));
	return true;
}

static bool cipserver_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}


static bool cipservermaxconn_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool cipsto_setup(char msg[])
{
	memset(txBuffer,0,sizeof(txBuffer));
	strcpy(txBuffer, pattern_OK);
    return true;
}

//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
 * @brief	Function handle of all at wi-fi commands.
 * @param   msg[in]	- Received at message.
 * @return  true/false.
**/
bool esp_tcp_handle(char msg[])
{  
    if(strstr(msg, pattern_CIPSTATUS_Cmd))
    {
        cipstatus_request(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPSTART_Cmd))
    {
        cipstart_setup(msg);
        return true;
    }

//    if(strstr(msg, pattern_CIPSTART_Query))
//    {
//        cipstart_request(msg);
//        return true;
//    }


    if(strstr(msg, pattern_CIPCLOSE_S_Cmd))
    {
        cipclose_s_setup(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPCLOSE_M_Cmd))
    {
        cipclose_m_setup(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPMUX_Cmd))
    {
        cipmux_setup(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPMUX_Query))
    {
        cipmux_request(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPMODE_Cmd))
    {
        cipmode_setup(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPMODE_Query))
    {
        cipmode_request(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPSERVER_Cmd))
    {
        cipserver_setup(msg);
        return true;
    }


    if(strstr(msg, pattern_CIPSERVERMAXCONN_Cmd))
    {
        cipservermaxconn_setup(msg);
        return true;
    }

    if(strstr(msg, pattern_CIPSTO_Cmd))
    {
        cipsto_setup(msg);
        return true;
    }

    transmit_error();
    return false;
}
#endif
