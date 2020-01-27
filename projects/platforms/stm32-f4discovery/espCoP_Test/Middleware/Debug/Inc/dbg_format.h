/********************************************************************************
*
* @file    		xprintf.h
* @author  		ChaN
* @website		http://elm-chan.org/profile_e.html
* @email  		user5@elm-chan.org
* @brief   		Universal String Handler for Console Input and Output
*
* Copyright (C) 2012, ChaN, all right reserved.
*
* xprintf module is an open source software. Redistribution and use of
* xprintf module in source and binary forms, with or without modification,
* are permitted provided that the following condition is met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* 	 this condition and the following disclaimer.
*
* This software is provided by the copyright holder and contributors "AS IS"
* and any warranties related to this software are DISCLAIMED.
* The copyright owner or contributors be NOT LIABLE for any damages caused
* by use of this software.
*
********************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdarg.h>
//_____ C O N F I G S  ________________________________________________________
#define _USE_XFUNC_OUT		1													/* 1: Use output functions */
#define	_CR_CRLF			1													/* 1: Convert \n ==> \r\n in the output char */
#define	_USE_LONGLONG		0													/* 1: Enable long long integer in type "ll". */
#define	_LONGLONG_t			long long											/* Platform dependent long long integer type */

#define _USE_XFUNC_IN		1													/* 1: Use input function */
#define	_LINE_ECHO			1													/* 1: Echo back input chars in xgets function */
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
void select_specified_device(void(*func)(unsigned char));

#if _USE_XFUNC_OUT
#define xdev_out(func) xfunc_out = (void(*)(unsigned char))(func)
extern void (*xfunc_out)(unsigned char);
void xputc (char c);

void xvprintf(const char* fmt, va_list arp);

void xputs (const char* str);
void xfputs (void (*func)(unsigned char), const char* str);
void xprintf (const char* fmt, ...);
void xsprintf (char* buff, const char* fmt, ...);
void xfprintf (void (*func)(unsigned char), const char*	fmt, ...);
void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)
#endif

#if _USE_XFUNC_IN
#define xdev_in(func) xfunc_in = (unsigned char(*)(void))(func)
extern unsigned char (*xfunc_in)(void);
int xgets (char* buff, int len);
int xfgets (unsigned char (*func)(void), char* buff, int len);
int xatoi (char** str, long* res);
#endif

