/********************************************************************************
*
* @file    		esp_basic.h
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		API for ESP8266 Basic AT Commands.
*
********************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdbool.h>
#include <stdint.h>
#include "esp_utils.h"
//_____ C O N F I G S  ________________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
//! @brief List of wake up pin modes
//! @{
typedef enum
{
	ESP8266_Enable           		=       '1',								///< ESP8266 can be woken up from light-sleep by GPIO
	ESP8266_Disable          		=       '0',								///< ESP8266 can NOT be woken up from light-sleep by GPIO
}	ESP_WakeupPinEnable_t;
//! @}

//! @brief Available levels for wake up pin
//! @{
typedef enum
{
	ESP_WakeupPinLowLevel           =       '0',								///< The GPIO wakes up ESP8266 on low level
	ESP_WakeupPinHighLevel          =       '1',								///< The GPIO wakes up ESP8266 on high level
}	ESP_WakeupPinLevel_t;
//! @}

//! @brief Available levels for awake up pin
//! @{
typedef enum
{
	ESP_AwakeupPinLowLevel           =       '0',								///< The GPIO is set to be low level after the wakeup process
	ESP_AwakeupPinHighLevel          =       '1',								///< The GPIO is set to be high level after the wakeup process
}	ESP_AwakeupPinLevel_t;
//! @}

//! @brief List of available data bits count
//! @{
typedef enum
{
	ESP_DataBits_5           		=       '5',								///< 5-bit data
	ESP_DataBits_6           		=       '6',								///< 6-bit data
	ESP_DataBits_7           		=       '7',								///< 7-bit data
	ESP_DataBits_8           		=       '8',								///< 8-bit data
}	ESP_DataBits_t;
//! @}

//! @brief List of available stop bits count
//! @{
typedef enum
{
	ESP_DataBits_1           		=       '1',								///< 1-bit stop bit
	ESP_DataBits_1_5         		=       '2',								///< 1.5-bit stop bit
	ESP_DataBits_2           		=       '3',								///< 2-bit stop bit
}	ESP_StopBits_t;
//! @}

//! @brief List of available parity check modes
//! @{
typedef enum
{
	ESP_ParityNone           		=       '0',								///< None parity check
	ESP_ParityOdd         	 		=       '1',								///< Odd parity check
	ESP_ParityEven           		=       '2',								///< Even parity check
}	ESP_Parity_t;
//! @}

//! @brief List of available flow control modes
//! @{
typedef enum
{
	ESP_FlowControlOff       		=       '0',								///< Flow control is not enabled
	ESP_FlowControlRTS       		=       '1',								///< Enable RTS
	ESP_FlowControlCTS       		=       '2',								///< Enable CTS
	ESP_FlowControlBoth      		=       '3',								///< Enable both RTS and CTS
}	ESP_FlowControl_t;
//! @}

//! @brief List of available sleep modes
//! @{
typedef enum
{
	ESP_SleepModeOff       	 		=       '0',								///< Disables sleep mode
	ESP_SleepModeLight       		=       '1',								///< Light-sleep mode
	ESP_SleepModeModem       		=       '2',								///< Modem-sleep mode
}	ESP_SleepModes_t;
//! @}

//! @brief Params for UART config
//! @{
typedef struct
{
	uint32_t baudRate;															///< Speed of UART port
	ESP_DataBits_t dataBits;													///< Count of data bits
	ESP_StopBits_t stopBits;													///< Count of stop bits
	ESP_Parity_t parity;														///< Enable/disable parity check
	ESP_FlowControl_t flowControl;												///< Enable/disable flow control
}	ESP_UartParam_t;
//! @}

//! @brief Params for wakeup gpio config
//! @{
typedef struct
{
	ESP_WakeupPinEnable_t enable;												///< Enable/disable wake up pin
	uint8_t trigger_GPIO;														///< Sets the GPIO to wake ESP8266 up; range of value: [0, 15].
	ESP_WakeupPinLevel_t trigger_level;											///< Trigger level for wake up pin
	uint8_t awake_GPIO;															///< Set a GPIO as a flag of ESP8266’s being awoken form Light-sleep
	ESP_AwakeupPinLevel_t awake_level;											///< Level of pin after the wake up process
}	ESP_WakeupGpioParam_t;
//! @}

//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
* @brief 	This function used to test the setup function of your wireless
* 			WiFi module.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_Test(uint32_t timeout);

/**
* @brief 	This function used used to restart the module.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_Reset(uint32_t timeout);

/**
* @brief 	This function return the version of AT commands and SDK that you are using.
*
* @param[out] *at AT commands version.
* @param[out] *sdk SDK version.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_GetVersion(ESP_AtVersion *at, ESP_SdkVersion *sdk, uint32_t timeout);

/**
* @brief 	This function is used to invoke the deep-sleep mode of the module.
*
* @param[in] time time is ms. ESP8266 will wake up after X ms and then
*			 enter the deep sleep mode
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_EnterToDeepSleep(uint32_t time, uint32_t timeout);

/**
* @brief 	This function enable echo with module answers.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_EnableEcho(uint32_t timeout);

/**
* @brief 	This function disable echo with module answers.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_DisableEcho(uint32_t timeout);

/**
* @brief 	This function This command is used to reset all parameters saved in flash.
*			restore the factory default settings of the module.
*
* @warning	The chip will be restarted when this command is executed.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_Restore(uint32_t timeout);

/**
* @brief 	This function sets the UART configuration and writes the new configuration
* 			to the flash. It is stored as the default parameter and will also be used
* 			as the default baudrate henceforth
*
* @warning	This API is deprecated, please use AT+UART_CUR or AT+UART_DEF instead.
* @warning  Flow control needs hardware support: MTCK is UART0 CTS and MTDO is UART0 RTS
*
* @param[in] cfg UART param structure.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupUartParam(const ESP_UartParam_t *cfg, uint32_t timeout);

/**
* @brief 	This function sets the current UART configuration; it does not write to
* 			the flash device. Hence there is no change in the default baudrate.
*
* @note		This configuration will NOT be stored in the flash; unlike the AT+UART
*			command.
* @note  	Flow control needs hardware support: MTCK is UART0 CTS and MTDO is UART0 RTS
*
* @param[in] cfg UART param structure.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupUartParamCur(const ESP_UartParam_t *cfg, uint32_t timeout);

/**
* @brief 	This function sets the UART configuration and save it to flash.
* 			It is stored as the default parameter and will also be used as
* 			the default baudrate henceforth.
*
* @note		This configuration will be stored in the flash user parameter area.
* @note  	Flow control needs hardware support: MTCK is UART0 CTS and MTDO is UART0 RTS
*
* @param[in] cfg UART param structure.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupUartParamDef(const ESP_UartParam_t *cfg, uint32_t timeout);

/**
* @brief 	This function sets ESP8266 sleep mode.
* 			It can only be used in station mode default to be modem sleep mode.
*
* @param[in] mode type of sleep mode.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_Sleep(ESP_SleepModes_t mode, uint32_t timeout);

/**
* @brief 	This function configures a GPIO to wake ESP8266 up from Light-sleep mode.
*
* @param[in] gpio gpio params see @ESP_WakeupGpioParam_t.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_ConfigWakeupGpio(const ESP_WakeupGpioParam_t *gpio, uint32_t timeout);

/**
* @brief 	This function sets the maximum value of the RF TX Power.
*
* @param[in] power maximum value of the RF TX Power.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupRfPower(uint8_t power, uint32_t timeout);

/**
* @brief 	This function sets current system messages.
*
* @param[in] msg sets current system messages.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupSystemMessageCur(uint8_t msg, uint32_t timeout);

/**
* @brief 	This function sets current system messages.
*
* @param[in] msg sets current system messages.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupSystemMessageDef(uint8_t msg, uint32_t timeout);
