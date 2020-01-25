// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/**********************************************************************************************
* @file    		QAdfifo.c
* @author  		Kovalchuk Alexander aka mz4
* @email 		roux@yandex.ru
* @version 		0.1
* @date    		06.06.2018
* @brief		Functions for testing dfifo library.
***********************************************************************************************/
//_____ I N C L U D E S _______________________________________________________________________
#include <esp_tcpip.h>
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "cmockery.h"
#include <string.h>
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________________________
static void Test_One(void **state)
{

}

//_____ F U N C T I O N   D E F I N I T I O N   _______________________________________________
void Test_EspTcpStack(void)
{
  const UnitTest tests[] =
  {
	  unit_test(Test_One),
  };

  run_tests(tests);
}
