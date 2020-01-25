// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/*******************************************************************************
* @file    		esp_drv.c
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
//_____ I N C L U D E S ________________________________________________________
#include <esp_tcpip.h>
#include <qa_port.h>
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "debug.h"
#include <stack/esp_basic.h>
#include <stack/esp_wifi.h>
#include "esp_drv.h"
#include "assert.h"
//_____ D E F I N I T I O N ____________________________________________________
#define SSID	"HUAWEI-DUge\0"
#define PASS	"485754439FED179D\0"
//_____ M A C R O S ____________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  _________________________________
static char message[] = "Hello\r\n";
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   __________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   ___________________
static bool receice_handlle(ESP_ConnectionId_t id, const char data[], size_t size)
{
	if(data == NULL || size == 0)
	{
		debug_error("[TCP]: Received from id: %c...FAULT\r\n", id);
		return false;
	}

	if(size == 1 && data[0] == 0xAA)
	{
		debug_info("[TCP]: Switch off the transparent mode...");
		debug_info("\r\n");
		ESPStatus_t res = ESP_TransparentModeDisable(5000u);
		if(res == ESP_PASS)
		{
			debug_info("PASS\r\n");
		}
		else
		{
			debug_error("FAULT\r\n");
		}
		ESP_HardWareSwitchMode(UNBLOCK);
	}
	else
	{
		debug_info("[TCP]: Received from id: %c\r\n", id);
		debug_info("\r\n");
		debug_dump((const void*)data, (unsigned long)&data[0], size, 1);
		debug_info("\r\n");
		debug_info("\r\n");

		ESP_TransmitData(id, message, sizeof(message));
	}


	return true;
}

//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
bool ESP_InitExample2(void)
{
	ESP_RegisterReceiceCB(&receice_handlle);
	return ESP_Init();
}

void ESP_HandleExample2(void)
{
	const char ssid[] = SSID;
	const char password[] = PASS;
	char ssid_cur[35] = {0};
	Ipv4Addr_t ip = 0;
	Ipv4Addr_t gw = 0;
	Ipv4Addr_t mask = 0;
	TCP_ConnectonParam_t connParam;
	char ipStr[] = "192.168.100.7\0";
	char _ip[] = "000.000.000.000\0";
	char _gw[] = "000.000.000.000\0";
	char _msk[] = "000.000.000.000\0";
	ESP_ConnectonStatus_t status;
	ESP_TransferMode_t transfer;
	ESP_ConnectionMode_t mux;
	uint32_t count = 3;
	ESPStatus_t res = ESP_INNER_ERR;

	Convert_StringToIpv4addr(&connParam.remoteIp, ipStr);
	connParam.remotePort = 9000;
	connParam.id = ID_NONE;

	debug_info("Example #2\r\n");
	debug_info("- WiFi in station mode\r\n");
	debug_info("- TCP client\r\n");
	debug_info("- Single connection mode\r\n");
	debug_info("- Transparent transmit mode\r\n");
	debug_info("\r\n");
	debug_info("\r\n");


	debug_info("Setup WiFi station mode...");
	if(ESP_SetupWifiModeCur(WIFI_CLIENT, 5000u))
	{
		debug_info(" PASS\r\n");

		debug_info("Check connection to AP status: \r\n");
		if(ESP_RequestNameConnectedApCur(ssid_cur, 5000u) == ESP_PASS)
		{
			if(strlen(ssid_cur) != 0)
			{
				debug_info("\tjoin to %s\r\n", ssid_cur);
				if(ESP_UnJoinFromWifiAp(5000u))
				{
					debug_info("\tunjoin from %s\r\n", ssid_cur);
				}
			}
			else
			{
				debug_info("\tdo not join\r\n");
			}
		}

		debug_info("Join to SSID: %s...", ssid);
		while(res != ESP_PASS && count > 0)
		{
			res = ESP_JoinToWifiApCur(ssid, password, 5000u);
			count--;
		}

		if(res == ESP_PASS)
		{
			debug_info(" PASS\r\n");
			debug_info("Request station IP...");

			res = ESP_INNER_ERR;
			count = 1;
			while(res != ESP_PASS && count != 0)
			{
				res = ESP_RequestWifiStationIpAddrCur(&ip, &gw, &mask, 5000u);
				count++;
			}
			if(res == ESP_PASS)
			{
				debug_info(" PASS\r\n");

				Convert_Ipv4addrToString(ip, _ip);
				Convert_Ipv4addrToString(gw, _gw);
				Convert_Ipv4addrToString(mask, _msk);

				debug_info("\tIP: %s\r\n", _ip);
				debug_info("\tGW: %s\r\n", _gw);
				debug_info("\tMASK: %s\r\n", _msk);
			}
			else
			{
				debug_error(" FAULT\r\n");
			}

			debug_info("Request multiplexor mode...");
			res = ESP_INNER_ERR;
			count = 0;

			while(res != ESP_PASS)
			{
				res = ESP_RequestConfigMux(&mux, 5000u);
				count++;
			}
			if(res == ESP_PASS)
			{
				debug_info(" PASS\r\n");
				if(mux != SINGLE_CONNECT)
				{
					debug_info("Setup single mode...");
					if(ESP_SingleConnectionEnable(5000u) == ESP_SERVER_ON)
					{
						debug_info(" FAULT\r\n");
						debug_info("\t Need disable TCP Server...");
						if(ESP_CloseServerTCP(0, 5000u))
						{
							debug_info(" PASS\r\n");

							debug_info("Setup single mode...");
							if(ESP_SingleConnectionEnable(5000u))
							{
								debug_info(" PASS\r\n");
							}
							else
							{
								debug_error(" FAULT\r\n");
							}
						}
						else
						{
							debug_error(" FAULT\r\n");
						}
					}
					else
					{
						debug_info(" PASS\r\n");
					}
				}
				else
				{
					debug_info("\tAlready in single mode...\r\n");
				}

				debug_info("Request transfer mode...");

				res = ESP_INNER_ERR;
				count = 0;
				while(res != ESP_PASS)
				{
					res = ESP_RequestTransferMode(&transfer, 5000u);
					count++;
				}
				if(res == ESP_PASS)
				{
					debug_info(" PASS\r\n");
					if(transfer != TRANSPARENT_MODE)
					{
						debug_info("Setup transparent transfer mode...");
						if(ESP_SetupTransferMode(TRANSPARENT_MODE, 5000u) == ESP_PASS)
						{
							debug_info(" PASS\r\n");
						}
						else
						{
							debug_error(" FAULT\r\n");
						}
					}
					else
					{
						debug_info("\tAlready in transparent transfer mode...\r\n");
					}
				}
				else if(res == ESP_PASS)
				{
					debug_error(" TIMEOUT\r\n");
				}
				else
				{
					debug_error(" FAULT\r\n");
				}



				debug_info("Establish TCP connection...");
				if(ESP_EstablishConnectTCP(&connParam, 5000u) == ESP_PASS)
				{
					debug_info(" PASS\r\n");
					if(ESP_RequestConnectionStatus(&status, 5000u))
					{
						debug_info("\tID: %c\r\n", status.id);
						debug_info("\tlocalPort: %x\r\n", status.localPort);
						Convert_Ipv4addrToString(status.remoteIp, _ip);
						debug_info("\tremoteIp: %s\r\n", _ip);
						debug_info("\tremotePort: %x\r\n", status.remotePort);
						debug_info("\tstat: %c\r\n", status.stat);
						debug_info("\ttype: %x\r\n", status.type);
					}
				}
				else
				{
					debug_error(" FAULT\r\n");
				}

				ESP_HardWareSwitchMode(UNBLOCK);
				ESP_TransmitData(ID_NONE, message, sizeof(message));
			}
			else
			{
				debug_error(" FAULT\r\n");
			}
		}
		else
		{
			debug_error(" FAULT\r\n");
		}
	}

	while (1)
	{
		ESP_MsgHandlCallBack();
		ESP_TransmitDataHandle();
	}
}


