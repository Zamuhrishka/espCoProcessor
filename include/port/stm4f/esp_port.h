/********************************************************************************
*
* @file    		esp_port.h
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief
*
********************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include "esp_queue.h"
#include "esp_drv.h"
#include "esp_utils.h"
#include <stdbool.h>
#include <stdint.h>
//_____ C O N F I G S  ________________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
#define BUFFER_SIZE							ESP_PAYLOAD_LENGTH
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
void ESP_HardWareInit(void);
void ESP_HardWareSwitchMode(Blocking_t mode);
bool ESP_HardWarePowerOn(void);
bool ESP_HardWarePowerOff(void);
bool ESP_HardWareTransmitUartBlock(const char data[], uint16_t size);
ESPStatus_t ESP_HardWareReceiveUartBlock(uint8_t* data, uint16_t size, uint32_t timeout);
void ESP_HardWareReceiveIrq(void);
