/********************************************************************************
* @file    		esp_wifi.h
* @author  		Kovalchuk Alexander
* @email  		alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
********************************************************************************/
#if defined(UNIT_TESTING)
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdint.h>
#include <stdbool.h>
//_____ C O N F I G S  ________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I С  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
bool esp_basic_handle(char msg[]);
#endif
