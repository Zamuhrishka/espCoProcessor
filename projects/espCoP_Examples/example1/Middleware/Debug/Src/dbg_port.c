// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		dbg_port.c
* @author  		Kovalchuk Alexander - 	alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
* 				which use for adding debug functionality
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include "main.h"
#include <stdbool.h>
#include "stdlib.h"
#include "string.h"
#include <stdarg.h>

#include "stm32f4xx_hal.h"

#include "dbg_format.h"
#include "dbg_port.h"
#include "debug.h"
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
#ifdef DEBUG

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;

/// Queue structure
static struct
{
	unsigned char data[DEBUG_BUFFER_SIZE];										///< Inner buffer
	volatile size_t quantity;													///< Count of stored data
	volatile size_t write;														///< Pointer to the write position
	volatile size_t read;														///< Pointer to the read position
} 	debugBuffer;

static unsigned char dbgOutput[DEBUG_BUFFER_SIZE] = {0};

//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
/*!
 @brief 	This function add new data in to the ring buffer.

 @param  	c[in] - value for saving.

* @return 	true/false.
*/
inline static bool debug_buffer_enqueue(unsigned char c)
{
	if(debugBuffer.quantity == DEBUG_BUFFER_SIZE) {
		return false;
	}

	debugBuffer.data[debugBuffer.write] = c;
	debugBuffer.quantity++;
	debugBuffer.write = (debugBuffer.write == DEBUG_BUFFER_SIZE - 1u) ? 0ul: (debugBuffer.write + 1u);

	return true;
}

/*!
 @brief 	This function return data from the ring buffer.

 @param  	*c[out] - pointer to the variable in which need to be return value.

* @return 	true/false.
*/
inline static bool debug_buffer_denqueue(unsigned char *c)
{
	if(debugBuffer.quantity == 0ul) {
		return false;
	}

	(*c) = debugBuffer.data[debugBuffer.read];
	debugBuffer.quantity--;
	debugBuffer.read = (debugBuffer.read == DEBUG_BUFFER_SIZE - 1ul) ? 0ul : (debugBuffer.read + 1ul);

	return true;
}

/*!
 @brief 	This function transmit data from buffer into debug UART.

 @param  	none.

* @return 	none.
*/
inline static void debug_transmit(void)
{
	size_t nbm = 0;

	nbm = debugBuffer.quantity;

	for(size_t i = 0; i < nbm; i++) {
		debug_buffer_denqueue(&dbgOutput[i]);
	}

	HAL_UART_Transmit_DMA(&huart2, dbgOutput, nbm);
	while(HAL_DMA_GetState(&hdma_usart2_tx) != HAL_DMA_STATE_READY) {};
	while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) != true) {};
}
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________
static void xdev_std_out(unsigned char c)
{
	debug_buffer_enqueue(c);
	if(c == '\n') {
		debug_transmit();
	}
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/*!
* @brief 	Initialization of debuging port.
*
* @param  	none.
*
* @return 	none.
*/
void dbg_port_init(void)
{
	xdev_out(xdev_std_out);
}
#else

void dbg_port_init(void)
{
	((void)(0));
}

#endif

