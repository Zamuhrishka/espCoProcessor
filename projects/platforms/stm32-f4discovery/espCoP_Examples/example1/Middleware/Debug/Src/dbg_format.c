// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		xprintf.c
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
//_____ I N C L U D E S _______________________________________________________
#include <dbg_format.h>
#if _USE_XFUNC_OUT
#include <stdarg.h>
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
void (*xfunc_out)(unsigned char);												/* Pointer to the output stream */
static char *outptr;
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
/**
* @brief 	Formatted string output.
*
* @param  	*fmt[in] - Pointer to the format string.
* @param  	arp[in] - ointer to arguments.
*
* @return 	none.
*
* @example	xprintf("%d", 1234);			"1234"
			xprintf("%6d,%3d%%", -200, 5);	"  -200,  5%"
			xprintf("%-6u", 100);			"100   "
			xprintf("%ld", 12345678);		"12345678"
			xprintf("%llu", 0x100000000);	"4294967296"	<_USE_LONGLONG>
			xprintf("%04x", 0xA3);			"00a3"
			xprintf("%08lX", 0x123ABC);		"00123ABC"
			xprintf("%016b", 0x550F);		"0101010100001111"
			xprintf("%s", "String");		"String"
			xprintf("%-5s", "abc");			"abc  "
			xprintf("%5s", "abc");			"  abc"
			xprintf("%c", 'a');				"a"
			xprintf("%f", 10.0);            <xprintf lacks floating point support. Use regular printf.>
*/
/*static*/ void xvprintf(const char* fmt, va_list arp)
{
	unsigned int r, i, j, w, f;
	char s[24], c, d, *p;
#if _USE_LONGLONG
	_LONGLONG_t v;
	unsigned _LONGLONG_t vs;
#else
	long v;
	unsigned long vs;
#endif


	for (;;) {
		c = *fmt++;																	/* Get a format character */
		if (!c) break;																/* End of format? */
		if (c != '%') {																/* Pass it through if not a % sequense */
			xputc(c); continue;
		}
		f = 0;																		/* Clear flags */
		c = *fmt++;																	/* Get first char of the sequense */
		if (c == '0') {																/* Flag: left '0' padded */
			f = 1; c = *fmt++;
		} else {
			if (c == '-') {															/* Flag: left justified */
				f = 2; c = *fmt++;
			}
		}
		for (w = 0; c >= '0' && c <= '9'; c = *fmt++) {								/* Minimum width */
			w = w * 10 + c - '0';
		}
		if (c == 'l' || c == 'L') {													/* Prefix: Size is long */
			f |= 4; c = *fmt++;
#if _USE_LONGLONG
			if (c == 'l' || c == 'L') {												/* Prefix: Size is long long */
				f |= 8; c = *fmt++;
			}
#endif
		}
		if (!c) break;																/* End of format? */
		d = c;
		if (d >= 'a') d -= 0x20;
		switch (d) {																/* Type is... */
		case 'S' :																	/* String */
			p = va_arg(arp, char*);
			for (j = 0; p[j]; j++) ;
			while (!(f & 2) && j++ < w) xputc(' ');
			xputs(p);
			while (j++ < w) xputc(' ');
			continue;
		case 'C' :																	/* Character */
			xputc((char)va_arg(arp, int)); continue;
		case 'B' :																	/* Binary */
			r = 2; break;
		case 'O' :																	/* Octal */
			r = 8; break;
		case 'D' :																	/* Signed decimal */
		case 'U' :																	/* Unsigned decimal */
			r = 10; break;
		case 'X' :																	/* Hexdecimal */
			r = 16; break;
		default:																	/* Unknown type (passthrough) */
			xputc(c); continue;
		}

		/* Get an argument and put it in numeral */
#if _USE_LONGLONG
		if (f & 8) {																/* long long argument? */
			v = va_arg(arp, _LONGLONG_t);
		} else {
			if (f & 4) {															/* long argument? */
				v = (d == 'D') ? (long)va_arg(arp, long) : (long)va_arg(arp, unsigned long);
			} else {																/* int/short/char argument */
				v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
			}
		}
#else
		if (f & 4) {																/* long argument? */
			v = va_arg(arp, long);
		} else {																	/* int/short/char argument */
			v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
		}
#endif
		if (d == 'D' && v < 0) {													/* Negative value? */
			v = 0 - v; f |= 16;
		}
		i = 0; vs = v;
		do {
			d = (char)(vs % r); vs /= r;
			if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
			s[i++] = d + '0';
		} while (vs != 0 && i < sizeof s);
		if (f & 16) s[i++] = '-';
		j = i; d = (f & 1) ? '0' : ' ';
		while (!(f & 2) && j++ < w) xputc(d);
		do xputc(s[--i]); while (i != 0);
		while (j++ < w) xputc(' ');
	}
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* @brief 	Put a character.
*
* @param  	c[in] - character.
*
* @return 	none.
*/
void xputc(char c)
{
	//if (_CR_CRLF && c == '\n') xputc('\r');		/* CR -> CRLF */

	if (outptr) {																	/* Destination is memory */
		*outptr++ = (unsigned char)c;
		return;
	}
	if (xfunc_out) {																/* Destination is device */
		xfunc_out((unsigned char)c);
	}
}

/**
* @brief 	Put a null-terminated string to the default device.
*
* @param  	str[in] - Pointer to the string.
*
* @return 	none.
*/
void xputs(const char* str)
{
	while (*str) {
		xputc(*str++);
	}
}


void select_specified_device(void(*func)(unsigned char))
{
	xfunc_out = func;																/* Switch output to specified device */
}


/**
* @brief 	Put a null-terminated string to the specified device.
*
* @param  	*func[in] - Pointer to the output function.
* @param  	str[in] - Pointer to the string.
*
* @return 	none.
*/
void xfputs(void(*func)(unsigned char), const char* str)
{
	void (*pf)(unsigned char);

	pf = xfunc_out;																	/* Save current output device */
	xfunc_out = func;																/* Switch output to specified device */
	while (*str) {																	/* Put the string */
		xputc(*str++);
	}
	xfunc_out = pf;																	/* Restore output device */
}

/**
* @brief 	Put a formatted string to the default device.
*
* @param  	fmt[in] - Pointer to the format string.
* @param  	...[in] - Optional arguments.
*
* @return 	none.
*/
void xprintf(const char* fmt, ...)
{
	va_list arp;

	va_start(arp, fmt);
	xvprintf(fmt, arp);
	va_end(arp);
}

/**
* @brief 	Put a formatted string to the memory.
*
* @param  	buff[in] - Pointer to the output buffer.
* @param  	fmt[in] - Pointer to the format string.
* @param  	...[in] - Optional arguments.
*
* @return 	none.
*/
void xsprintf(char* buff, const char* fmt, ...)
{
	va_list arp;

	outptr = buff;																	/* Switch destination for memory */

	va_start(arp, fmt);
	xvprintf(fmt, arp);
	va_end(arp);

	*outptr = 0;																	/* Terminate output string with a \0 */
	outptr = 0;																		/* Switch destination for device */
}

/**
* @brief 	Put a formatted string to the specified device.
*
* @param  	*func[in] - Pointer to the output function.
* @param  	fmt[in] - Pointer to the format string.
* @param  	...[in] - Optional arguments.
*
* @return 	none.
*/
void xfprintf(void(*func)(unsigned char), const char* fmt, ...)
{
	va_list arp;
	void (*pf)(unsigned char);

	pf = xfunc_out;																	/* Save current output device */
	xfunc_out = func;																/* Switch output to specified device */

	va_start(arp, fmt);
	xvprintf(fmt, arp);
	va_end(arp);

	xfunc_out = pf;																	/* Restore output device */
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
void put_dump(const void* buff,	unsigned long addr,	int len, int width)
{
	int i;
	const unsigned char *bp;
	const unsigned short *sp;
	const unsigned long *lp;


	xprintf("%08lX ", addr);														/* address */

	switch (width) {
	case DW_CHAR:
		bp = buff;
		for (i = 0; i < len; i++)													/* Hexdecimal dump */
			xprintf(" %02X", bp[i]);
		xputc(' ');
		for (i = 0; i < len; i++)													/* ASCII dump */
			xputc((unsigned char)((bp[i] >= ' ' && bp[i] <= '~') ? bp[i] : '.'));
		break;
	case DW_SHORT:
		sp = buff;
		do																			/* Hexdecimal dump */
			xprintf(" %04X", *sp++);
		while (--len);
		break;
	case DW_LONG:
		lp = buff;
		do																			/* Hexdecimal dump */
			xprintf(" %08LX", *lp++);
		while (--len);
		break;
	}

	xputc('\r');
	xputc('\n');
}

#endif /* _USE_XFUNC_OUT */



#if _USE_XFUNC_IN
unsigned char (*xfunc_in)(void);													/* Pointer to the input stream */

/**
* @brief 	Get a line from the default input.
*
* @param  	*buff[in] - Pointer to the buffer.
* @param  	len[in] - Buffer length.
*
* @return 	0:End of stream, 1:A line arrived.
*/
int xgets(char* buff, int len)
{
	int c, i;

	if (!xfunc_in) return 0;														/* No input function specified */

	i = 0;
	for (;;) {
		c = xfunc_in();																/* Get a char from the incoming stream */
		if (!c) return 0;															/* End of stream? */
		if (c == '\r') break;														/* End of line? */
		if (c == '\b' && i) {														/* Back space? */
			i--;
			if (_LINE_ECHO) xputc((unsigned char)c);
			continue;
		}
		if (c >= ' ' && i < len - 1) {												/* Visible chars */
			buff[i++] = c;
			if (_LINE_ECHO) xputc((unsigned char)c);
		}
	}
	buff[i] = 0;																	/* Terminate with a \0 */
	if (_LINE_ECHO) xputc('\n');
	return 1;
}

/**
* @brief 	Get a line from the specified input.
*
* @param  	*func[in] - Pointer to the input stream function.
* @param  	*buff[in] - Pointer to the buffer.
* @param  	len[in] - Buffer length.
*
* @return 	0:End of stream, 1:A line arrived.
*/
int xfgets(unsigned char (*func)(void),	char* buff,	int len)
{
	unsigned char (*pf)(void);
	int n;

	pf = xfunc_in;																	/* Save current input device */
	xfunc_in = func;																/* Switch input to specified device */
	n = xgets(buff, len);															/* Get a line */
	xfunc_in = pf;																	/* Restore input device */

	return n;
}

/**
* @brief 	Get a value of the string.
*
* @param  	**str[in] - Pointer to pointer to the string.
* @param  	*res[in] - Pointer to the valiable to store the value.
*
* @return 	0:Failed, 1:Successful.
*
* @example	"123 -5   0x3ff 0b1111 0377  w "
			^                           1st call returns 123 and next ptr
			   ^                        2nd call returns -5 and next ptr
					   ^                3rd call returns 1023 and next ptr
							  ^         4th call returns 15 and next ptr
								   ^    5th call returns 255 and next ptr
									  ^ 6th call fails and returns 0
*/
int xatoi(char **str, long *res)
{
	unsigned long val;
	unsigned char c, r, s = 0;

	*res = 0;

	while ((c = **str) == ' ') (*str)++;											/* Skip leading spaces */

	if (c == '-') {																	/* negative? */
		s = 1;
		c = *(++(*str));
	}

	if (c == '0') {
		c = *(++(*str));
		switch (c) {
		case 'x':																	/* hexdecimal */
			r = 16; c = *(++(*str));
			break;
		case 'b':																	/* binary */
			r = 2; c = *(++(*str));
			break;
		default:
			if (c <= ' ') return 1;													/* single zero */
			if (c < '0' || c > '9') return 0;										/* invalid char */
			r = 8;																	/* octal */
		}
	} else {
		if (c < '0' || c > '9') return 0;											/* EOL or invalid char */
		r = 10;																		/* decimal */
	}

	val = 0;
	while (c > ' ') {
		if (c >= 'a') c -= 0x20;
		c -= '0';
		if (c >= 17) {
			c -= 7;
			if (c <= 9) return 0;													/* invalid char */
		}
		if (c >= r) return 0;														/* invalid char for current radix */
		val = val * r + c;
		c = *(++(*str));
	}
	if (s) val = 0 - val;															/* apply sign if needed */

	*res = val;
	return 1;
}

#endif /* _USE_XFUNC_IN */
