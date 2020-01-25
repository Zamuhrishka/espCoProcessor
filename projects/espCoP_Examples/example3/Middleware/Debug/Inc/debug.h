/********************************************************************************
*
* @file    		debug.h
* @author  		Kovalchuk Alexander - 	alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
* 				which use for adding debug functionality
*
********************************************************************************/
#pragma once
//_____ D E F I N I T I O N ___________________________________________________
//_____ I N C L U D E S _______________________________________________________
//_____ C O N F I G S  ________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________________________
typedef enum
{
	DBG_FULL = 0,
	DBG_INFO,
	DBG_WARNING,
	DBG_ERROR,
	DBG_CRITICAL
}	DebugLevels_t;
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/*!
* @brief 	Initialization of debuging subsystem.
*
* @param  	none.
*
* @return 	none.
*/
void debug_init(void);

/*!
* @brief 	Switch on debuging.
*
* @param  	none.
*
* @return 	none.
*/
void debug_on(void);

/*!
* @brief 	Switch off debuging.
*
* @param  	none.
*
* @return 	none.
*/
void debug_off(void);

/*!
* @brief 	Setup debuging level.
*
* @param  	level[in] - active level of debuging.
* 			DBG_INFO - allow all type of debug message;
* 			DBG_WARNING - common debug messages switch off;
* 			DBG_ERROR - warning and common debug messages switch off;
* 			DBG_CRITICAL - allow only critical debug messages;
*
* @return 	none.
*/
void debug_set_level(DebugLevels_t level);

/*!
* @brief 	Return debuging level.
*
* @param  	none.
*
* @return 	active level of debuging.
*/
DebugLevels_t debug_get_level(void);

/*!
* @brief	Print common debug message into console.
*
* @param  	fmt[in] - debuging string.
*
* @return 	none.
*/
void debug_info(const char* fmt, ...);

/*!
* @brief	Print warning debug message into console.
*
* @param  	fmt[in] - debuging string.
*
* @return 	none.
*/
void debug_warning(const char* fmt, ...);

/*!
* @brief	Print error debug message into console.
*
* @param  	fmt[in] - debuging string.
*
* @return 	none.
*/
void debug_error(const char* fmt, ...);

/*!
* @brief	Print critical debug message into console.
*
* @param  	fmt[in] - debuging string.
*
* @return 	none.
*/
void debug_critical(const char* fmt, ...);

/**
* @brief 	Dump a line of binary dump.
*
* @param  	*buff[in] - Pointer to the array to be dumped.
* @param  	addr[in] - Heading address value.
* @param  	len[in] - Number of items to be dumped.
* @param  	width[in] - Size of the items (DF_CHAR, DF_SHORT, DF_LONG).
*
* @return 	none.
*/
void debug_dump(const void* buff,	unsigned long addr,	int len, int width);
