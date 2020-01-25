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

	debug_info("[TCP]: Received from id: %c\r\n", id);
	debug_info("\r\n");
	debug_dump((const void*)data, (unsigned long)&data[0], size, 1);
	debug_info("\r\n");
	debug_info("\r\n");
	ESP_TransmitData(id, message, sizeof(message));
	return true;
}

static bool disconnect_handlle(ESP_ConnectionId_t id)
{
	debug_info("[TCP]: Connection %c close\r\n", id);
	return true;
}

static bool connect_handlle(ESP_ConnectionId_t id)
{
	debug_info("[TCP]: Connection %c open\r\n", id);
	return true;
}

static void transmit_pass_handle(void)
{
	debug_info("[TCP]: Transmit Pass\r\n");
}
//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
bool ESP_InitExample4(void)
{
	ESP_RegisterReceiceCB(&receice_handlle);
	ESP_RegisterCloseConnectCB(&disconnect_handlle);
	ESP_RegisterOpenConnectCB(&connect_handlle);
	ESP_RegisterTransmitPassCB(&transmit_pass_handle);
	return ESP_Init();
}

void ESP_HandleExample4(void)
{
	ESP_WifiMode_t mode;
	ESPStatus_t res = ESP_INNER_ERR;
	Ipv4Addr_t ip = 0;
	Ipv4Addr_t gw = 0;
	Ipv4Addr_t mask = 0;
	char _ip[] = "000.000.000.000\0";
	char _gw[] = "000.000.000.000\0";
	char _msk[] = "000.000.000.000\0";
	ESP_ConnectonStatus_t status;
	ESP_TransferMode_t transfer;
	ESP_ConnectionMode_t mux;
	uint32_t count = 3;

	debug_info("Example #4\r\n");
	debug_info("- WiFi in AP mode\r\n");
	debug_info("- TCP server\r\n");
	debug_info("- Multi connection mode\r\n");
	debug_info("- Normal transmit mode\r\n");
	debug_info("\r\n");
	debug_info("\r\n");

	if(ESP_RequestWifiModeCur(&mode, 5000u))
	{
		if(ESP_SetupWifiModeCur(WIFI_AP, 5000u))
		{
			debug_info("Setup Wi-Fi AP mode pass\r\n");

			if(ESP_CreateWifiApCur("Mz4\0", "habrahabr\0", '5', '4', 5000u) == ESP_PASS)
			{
				debug_info("Create Wi-Fi AP...");

				if(ESP_RequestWifiApIpAddrCur(&ip, &gw, &mask, 5000u))
				{
					debug_error(" PASS\r\n");
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
					if(mux != MULTIPLE_CONNECT)
					{
						debug_info("Setup multi mode...");
						if(ESP_MultipleConnectionEnable(5000u) == ESP_SERVER_ON)
						{
							debug_info(" FAULT\r\n");
							debug_info("\t Need disable TCP Server...");
							if(ESP_CloseServerTCP(0, 5000u))
							{
								debug_info(" PASS\r\n");
								debug_info("Setup single mode...");
								if(ESP_MultipleConnectionEnable(5000u))
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
						debug_info("\tAlready in multi mode...\r\n");
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
						if(transfer != NORMAL_MODE)
						{
							debug_info("Setup normal transfer mode...");
							if(ESP_SetupTransferMode(NORMAL_MODE, 5000u) == ESP_PASS)
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
							debug_info("\tAlready in normal transfer mode...\r\n");
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

					debug_info("Create TCP server...");
					if(ESP_StartServerTCP(1001, 5000u) == ESP_PASS)
					{
						debug_info(" PASS\r\n");
						if(ESP_RequestConnectionStatus(&status, 5000u) == ESP_PASS)
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
				}
			}
			else
			{
				debug_error("Create Wi-Fi AP...fault\r\n");
			}
		}
		else
		{
			debug_error("Create Wi-Fi AP...fault\r\n");
		}
	}

	while (1)
	{
		ESP_MsgHandlCallBack();
		ESP_TransmitDataHandle();
	}
}


