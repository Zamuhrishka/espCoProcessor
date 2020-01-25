// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		debug.c
* @author  		Kovalchuk Alexander - 	alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
* 				which use for adding debug functionality
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include "stdlib.h"
#include "string.h"
#include <stdarg.h>
#include <stdbool.h>

#include "dbg_port.h"
#include "dbg_format.h"
#include "debug.h"

#ifdef DEBUG
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
static bool dbg_on = false;
static DebugLevels_t debug_level = DBG_FULL;
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/*!
* @brief 	Initialization of debuging subsystem.
*
* @param  	none.
*
* @return 	none.
*/
void debug_init(void)
{
	dbg_on = true;
	debug_level = DBG_FULL;
	dbg_port_init();
}

/*!
* @brief 	Switch on debuging.
*
* @param  	none.
*
* @return 	none.
*/
void debug_on(void)
{
	dbg_on = true;
}

/*!
* @brief 	Switch off debuging.
*
* @param  	none.
*
* @return 	none.
*/
void debug_off(void)
{
	dbg_on = false;
}

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
void debug_set_level(DebugLevels_t level)
{
	debug_level = level;
}

/*!
* @brief 	Return debuging level.
*
* @param  	none.
*
* @return 	active level of debuging.
*/
DebugLevels_t debug_get_level(void)
{
	return debug_level;
}

/*!
* @brief	Print common debug message into console.
*
* @param  	fmt[in] - debuging string.
*
* @return 	none.
*/
void debug_info(const char* fmt, ...)
{
	va_list arp;

	if(dbg_on)
	{
		if(debug_level <= DBG_INFO)
		{
			va_start(arp, fmt);
			xvprintf(fmt, arp);
			va_end(arp);
		}
	}
}

/*!
* @brief	Print warning debug message into console.
*
* @param  	fmt[in] - debuging string.
*
* @return 	none.
*/
void debug_warning(const char* fmt, ...)
{
	va_list arp;

	if(dbg_on)
	{
		if(debug_level <= DBG_WARNING)
		{
			xprintf("\033[33m");
			va_start(arp, fmt);
			xvprintf(fmt, arp);
			va_end(arp);
			xprintf("\033[0m");
		}
	}
}

/*!
* @brief	Print error debug message into console.
*
* @param  	fmt[in] - debuging string.
*
* @return 	none.
*/
void debug_error(const char* fmt, ...)
{
	va_list arp;

	if(dbg_on)
	{
		if(debug_level <= DBG_ERROR)
		{
			xprintf("\033[31m");
			va_start(arp, fmt);
			xvprintf(fmt, arp);
			va_end(arp);
			xprintf("\033[0m");
		}
	}
}

/*!
* @brief	Print critical debug message into console.
*
* @param  	fmt[in] - debuging string.
*
* @return 	none.
*/
void debug_critical(const char* fmt, ...)
{
	va_list arp;

	if(dbg_on)
	{
		if(debug_level <= DBG_CRITICAL)
		{
			xprintf("\033[91m\033[1m");
			va_start(arp, fmt);
			xvprintf(fmt, arp);
			va_end(arp);
			xprintf("\033[0m");
		}
	}
}

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
void debug_dump(const void* buff, unsigned long addr, int len, int width)
{
	put_dump(buff,addr, len, width);
}

#else
void debug_info(const char* fmt, ...)
{
	((void)(0));
}

void debug_warning(const char* fmt, ...)
{
	((void)(0));
}

void debug_error(const char* fmt, ...)
{
	((void)(0));
}

void debug_critical(const char* fmt, ...)
{
	((void)(0));
}

#endif
