/********************************************************************************
* @author  		Kovalchuk Alexander
* @email  		alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
* 				which use for...
********************************************************************************/
#if defined(UNIT_TESTING)
//_____ I N C L U D E S _______________________________________________________
#include "fake_basic.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "slre.h"
// #include "qa_port.h"
//_____ C O N F I G S  ________________________________________________________
#define  BUFFER_LENGTH             200u
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
extern char txBuffer[BUFFER_LENGTH];
static char buffer[BUFFER_LENGTH] = {0};

static char at_version[] = "\0";
static char sdk_version[] = "";
static char compile_time[] = "";

static char baurate[] = "115200";
static char databits[] = "8";
static char stopbits[] = "1";
static char parity[] = "0";
static char flowcontrol[] = "3";

static char sleep_mode[] = "";
//----------------------------------------------------------------------------------------------------
static char pattern_OK1[] = "\r\nOK\r\n";
static char pattern_ERROR1[] = "\r\nERROR\r\n";
//----------------------------------------------------------------------------------------------------
static char pattern_TEST_Cmd[] = "AT";
//----------------------------------------------------------------------------------------------------
static char pattern_RESTART_Cmd[] = "AT+RST";
//----------------------------------------------------------------------------------------------------
static char pattern_VERSION_Query[] = "AT+GMR";
//----------------------------------------------------------------------------------------------------
static char pattern_DEEP_SLEEP_Cmd[] = "AT+GSLP=";
//----------------------------------------------------------------------------------------------------
static char pattern_ECHO_Cmd[] = "ATE";
//----------------------------------------------------------------------------------------------------
static char pattern_RESTORE_Cmd[] = "AT+RESTORE";
//----------------------------------------------------------------------------------------------------
static char pattern_UART_CUR_Query[] = "AT+UART_CUR?";
static char pattern_UART_CUR_Cmd[] = "AT+UART_CUR=";
static char pattern_UART_CUR_Resp[] = "+UART_CUR:";
//----------------------------------------------------------------------------------------------------
static char pattern_UART_DEF_Query[] = "AT+UART_DEF?";
static char pattern_UART_DEF_Cmd[] = "AT+UART_DEF=";
static char pattern_UART_DEF_Resp[] = "+UART_DEF:";
//----------------------------------------------------------------------------------------------------
static char pattern_SLEEP_Query[] = "AT+SLEEP?";
static char pattern_SLEEP_Cmd[] = "AT+SLEEP=";
static char pattern_SLEEP_Resp[] = "+SLEEP:";
//----------------------------------------------------------------------------------------------------
static char pattern_WAKEUPGPIO_Cmd[] = "AT+WAKEUPGPIO=";
//----------------------------------------------------------------------------------------------------
static char pattern_RFPOWER_Cmd[] = "AT+RFPOWER=";
//----------------------------------------------------------------------------------------------------
static char pattern_SYSMSG_CUR_Cmd[] = "AT+SYSMSG_CUR=";
//----------------------------------------------------------------------------------------------------
static char pattern_SYSMSG_DEF_Cmd[] = "AT+SYSMSG_DEF=";
//----------------------------------------------------------------------------------------------------
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I С  F U N C T I O N   D E F I N I T I O N   __________________
static void transmit_ok(void)
{
	strcpy(txBuffer, pattern_OK1);
}

static bool test_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool restart_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool version_query(char msg[])
{
	strcpy(txBuffer, "AT version:1.1.0.0(May 11 2016 18:09:56)\r\nSDK version:1.5.4(baaeaebb)\r\ncompile time:May 20 2016 15:08:19\r\n\r\nOK\r\n");
    return true;
}

static bool deep_sleep_cmd(char msg[])
{
	struct slre_cap caps[1];

	if (slre_match((const char *)"AT\\+GSLP=(\\d+)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memcpy((void*)txBuffer, (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)(txBuffer + caps[0].len), "\r\n\r\nOK\r\n",strlen("\r\n\r\nOK\r\n"));

    return true;
}

static bool echo_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool restore_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool uart_cur_setup(char msg[])
{
	struct slre_cap caps[5];

	if (slre_match((const char *)"AT\\+UART_CUR=(\\d+)\\S(\\d+)\\S(\\d+)\\S(\\d+)\\S(\\d+)\r\n", msg, strlen(msg), caps, 5, 0) <= 0) {
	   return false;
	}

	memcpy((void*)baurate, (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)databits, (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)stopbits, (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)parity, (void*)caps[3].ptr, caps[3].len);
	memcpy((void*)flowcontrol, (void*)caps[4].ptr, caps[4].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool uart_cur_request(char msg[])
{
	uint32_t nbm = 0;

	memcpy((void*)txBuffer, "+UART_CUR:", strlen("+UART_CUR:"));
	memcpy((void*)(txBuffer + strlen("+UART_CUR:")), baurate, strlen(baurate));
	nbm = strlen("+UART_CUR:") + strlen(baurate);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), databits, strlen(databits));
	txBuffer[nbm + 2] = ',';
	memcpy((void*)(txBuffer + (nbm + 3)), stopbits, strlen(stopbits));
	txBuffer[nbm + 4] = ',';
	memcpy((void*)(txBuffer + (nbm + 5)), parity, strlen(parity));
	txBuffer[nbm + 6] = ',';
	memcpy((void*)(txBuffer + (nbm + 7)), flowcontrol, strlen(flowcontrol));
	memcpy((void*)(txBuffer + (nbm + 8)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

    return true;
}


static bool uart_def_setup(char msg[])
{
	struct slre_cap caps[5];

	if (slre_match((const char *)"AT\\+UART_DEF=(\\d+)\\S(\\d+)\\S(\\d+)\\S(\\d+)\\S(\\d+)\r\n", msg, strlen(msg), caps, 5, 0) <= 0) {
	   return false;
	}

	memcpy((void*)baurate, (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)databits, (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)stopbits, (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)parity, (void*)caps[3].ptr, caps[3].len);
	memcpy((void*)flowcontrol, (void*)caps[4].ptr, caps[4].len);

	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool uart_def_request(char msg[])
{
	uint32_t nbm = 0;

	memcpy((void*)txBuffer, "+UART_DEF:", strlen("+UART_DEF:"));
	memcpy((void*)(txBuffer + strlen("+UART_CUR:")), baurate, strlen(baurate));
	nbm = strlen("+UART_CUR:") + strlen(baurate);
	txBuffer[nbm] = ',';
	memcpy((void*)(txBuffer + (nbm + 1)), databits, strlen(databits));
	txBuffer[nbm + 2] = ',';
	memcpy((void*)(txBuffer + (nbm + 3)), stopbits, strlen(stopbits));
	txBuffer[nbm + 4] = ',';
	memcpy((void*)(txBuffer + (nbm + 5)), parity, strlen(parity));
	txBuffer[nbm + 6] = ',';
	memcpy((void*)(txBuffer + (nbm + 7)), flowcontrol, strlen(flowcontrol));
	memcpy((void*)(txBuffer + (nbm + 8)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

    return true;
}


static bool sleep_setup(char msg[])
{
	struct slre_cap caps[15];

	if (slre_match((const char *)"AT\\+SLEEP=(\\d+)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	memcpy((void*)sleep_mode, (void*)caps[0].ptr, caps[0].len);
	strcpy(txBuffer, pattern_OK1);

    return true;
}

static bool sleep_request(char msg[])
{
	uint32_t nbm = 0;

	memcpy((void*)txBuffer, "+SLEEP:", strlen("+SLEEP:"));
	memcpy((void*)(txBuffer + strlen("+UART_CUR:")), sleep_mode, strlen(sleep_mode));
	nbm = strlen("+SLEEP:") + strlen(sleep_mode);
	memcpy((void*)(txBuffer + (nbm)), "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));

    return true;
}

static bool wakeupgpio_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool rfpower_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK1);
    return true;
}

static bool sysmsg_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK1);
    return true;
}
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
 * @brief	Function handle of all at wi-fi commands.
 * @param   msg[in]	- Received at message.
 * @return  true/false.
**/
bool esp_basic_handle(char msg[])
{
//	if(strstr(msg, pattern_TEST_Cmd) != NULL)
//	{
//        test_cmd(msg);
//        return true;
//	}
	if(strstr(msg, pattern_RESTART_Cmd) != NULL)
	{
		restart_cmd(msg);
        return true;
	}
	if(strstr(msg, pattern_VERSION_Query) != NULL)
	{
		version_query(msg);
        return true;
	}
	if(strstr(msg, pattern_DEEP_SLEEP_Cmd) != NULL)
	{
		deep_sleep_cmd(msg);
        return true;
	}
	if(strstr(msg, pattern_ECHO_Cmd) != NULL)
	{
		echo_cmd(msg);
        return true;
	}
	if(strstr(msg, pattern_RESTORE_Cmd) != NULL)
	{
		restore_cmd(msg);
        return true;
	}
	if(strstr(msg, pattern_UART_CUR_Query) != NULL)
	{
		uart_cur_request(msg);
        return true;
	}
	if(strstr(msg, pattern_UART_CUR_Cmd) != NULL)
	{
		uart_cur_setup(msg);
        return true;
	}
	if(strstr(msg, pattern_UART_DEF_Query) != NULL)
	{
		uart_def_request(msg);
        return true;
	}
	if(strstr(msg, pattern_UART_DEF_Cmd) != NULL)
	{
		uart_def_setup(msg);
        return true;
	}
	if(strstr(msg, pattern_SLEEP_Query) != NULL)
	{
		sleep_request(msg);
        return true;
	}
	if(strstr(msg, pattern_SLEEP_Cmd) != NULL)
	{
		sleep_setup(msg);
        return true;
	}
	if(strstr(msg, pattern_WAKEUPGPIO_Cmd) != NULL)
	{
		wakeupgpio_cmd(msg);
        return true;
	}
	if(strstr(msg, pattern_RFPOWER_Cmd) != NULL)
	{
		rfpower_cmd(msg);
        return true;
	}
	if(strstr(msg, pattern_SYSMSG_CUR_Cmd) != NULL)
	{
		sysmsg_cmd(msg);
        return true;
	}

    return false;
 }
#endif
