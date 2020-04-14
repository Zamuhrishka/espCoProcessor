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
#define  BUFFER_LENGTH             200u
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
extern char txBuffer[BUFFER_LENGTH];

static const char pattern_OK[] = "\r\nOK\r\n";
static const char pattern_TEST_Cmd[] = "AT";
static const char pattern_RESTART_Cmd[] = "AT+RST";
static const char pattern_VERSION_Query[] = "AT+GMR";
static const char pattern_DEEP_SLEEP_Cmd[] = "AT+GSLP=";
static const char pattern_ECHO_Cmd[] = "ATE";
static const char pattern_RESTORE_Cmd[] = "AT+RESTORE";
static const char pattern_UART_CUR_Cmd[] = "AT+UART_CUR=";
static const char pattern_UART_DEF_Cmd[] = "AT+UART_DEF=";
static const char pattern_SLEEP_Cmd[] = "AT+SLEEP=";
static const char pattern_WAKEUPGPIO_Cmd[] = "AT+WAKEUPGPIO=";
static const char pattern_RFPOWER_Cmd[] = "AT+RFPOWER=";
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I С  F U N C T I O N   D E F I N I T I O N   __________________
static bool fake_esp_test_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_reset_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_version_request_cmd(char msg[])
{
	strcpy(txBuffer, "AT version:1.1.0.0(May 11 2016 18:09:56)\r\nSDK version:1.5.4(baaeaebb)\r\ncompile time:May 20 2016 15:08:19\r\n\r\nOK\r\n");
    return true;
}

static bool fake_esp_enter_deep_sleep_cmd(char msg[])
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

static bool fake_esp_echo_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_restore_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_uart_cfg_cur_cmd(char msg[])
{
	struct slre_cap caps[5];

	if (slre_match((const char *)"AT\\+UART_CUR=(\\d+)\\S(\\d+)\\S(\\d+)\\S(\\d+)\\S(\\d+)\r\n", msg, strlen(msg), caps, 5, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_uart_cfg_def_cmd(char msg[])
{
	struct slre_cap caps[5];

	if (slre_match((const char *)"AT\\+UART_DEF=(\\d+)\\S(\\d+)\\S(\\d+)\\S(\\d+)\\S(\\d+)\r\n", msg, strlen(msg), caps, 5, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_sleep_cmd(char msg[])
{
	struct slre_cap caps[15];

	if (slre_match((const char *)"AT\\+SLEEP=(\\d+)\r\n", msg, strlen(msg), caps, 1, 0) <= 0) {
	   return false;
	}

	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_config_wakeup_pins_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK);
    return true;
}

static bool fake_esp_setup_rf_power_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK);
    return true;
}

#if 0
static bool sysmsg_cmd(char msg[])
{
	strcpy(txBuffer, pattern_OK);
    return true;
}
#endif
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
bool fake_esp_basic_handle(char msg[])
{
	if(strstr(msg, pattern_RESTART_Cmd) != NULL)
	{
		fake_esp_reset_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_VERSION_Query) != NULL)
	{
		fake_esp_version_request_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_DEEP_SLEEP_Cmd) != NULL)
	{
		fake_esp_enter_deep_sleep_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_ECHO_Cmd) != NULL)
	{
		fake_esp_echo_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_RESTORE_Cmd) != NULL)
	{
		fake_esp_restore_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_UART_CUR_Cmd) != NULL)
	{
		fake_esp_uart_cfg_cur_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_UART_DEF_Cmd) != NULL)
	{
		fake_esp_uart_cfg_def_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_SLEEP_Cmd) != NULL)
	{
		fake_esp_sleep_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_WAKEUPGPIO_Cmd) != NULL)
	{
		fake_esp_config_wakeup_pins_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_RFPOWER_Cmd) != NULL)
	{
		fake_esp_setup_rf_power_cmd(msg);
        return true;
	}

	if(strstr(msg, pattern_TEST_Cmd) != NULL)
	{
		fake_esp_test_cmd(msg);
        return true;
	}

#if 0
	if(strstr(msg, pattern_SYSMSG_CUR_Cmd) != NULL)
	{
		sysmsg_cmd(msg);
        return true;
	}
#endif
    return false;
 }
#endif
