/********************************************************************************
* @author  		Kovalchuk Alexander
* @email  		alexander.kovalchuk@promwad.com
* @brief   		This file contains the prototypes functions and methods,
* 				which use for...
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include <qa_tcp.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "slre.h"
#include "qa_port.h"
//_____ C O N F I G S  ________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
#define  BUFFER_LENGTH             200u
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
extern char txBuffer[BUFFER_SIZE];

static char buffer[BUFFER_LENGTH] = {0};

static char type[] = "TCP";
static char linkID[] = "0";
static char remoteIP[] = "000.000.000.000";
static char remotePort[] = "9874";
static char localPortUDP[] = "9974";
static char modeUDP[] = "0";
static char keepAlive[] = "1000";

static TCPIP_Mux_t mux = MUX_SINGLE;
static char mode = '0';
//----------------------------------------------------------------------------------------------------
static char pattern_OK1[] = "\r\nOK\r\n";
static char pattern_ERROR1[] = "\r\nERROR\r\n";
//----------------------------------------------------------------------------------------------------
char pattern_CIPSTATUS_Cmd[] = "AT+CIPSTATUS";
char pattern_CIPSTATUS_Resp[] = "+CIPSTATUS:";
//----------------------------------------------------------------------------------------------------
char pattern_CIPSTART_Query[] = "AT+CIPSTART?";
char pattern_CIPSTART_Cmd[] = "AT+CIPSTART=";
char pattern_CIPSTART_Resp[] = "+CIPSTART:";

//----------------------------------------------------------------------------------------------------
char pattern_CIPMUX_Query[] = "AT+CIPMUX?";
char pattern_CIPMUX_Cmd[] = "AT+CIPMUX=";
char pattern_CIPMUX_Resp[] = "+CIPMUX:";
//----------------------------------------------------------------------------------------------------
char pattern_CIPMODE_Query[] = "AT+CIPMODE?";
char pattern_CIPMODE_Cmd[] = "AT+CIPMODE=";
char pattern_CIPMODE_Resp[] = "+CIPMODE:";
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I С  F U N C T I O N   D E F I N I T I O N   __________________
static void transmit_ok(void)
{
//    Serial1.print(pattern_OK);
//    Serial.print(pattern_OK);
}

static void transmit_error(void)
{
//    Serial1.print(pattern_ERROR);
//    Serial.print(pattern_ERROR);
}

static bool cipstatus_request(char msg[])
{
//    Serial1.print("STATUS:2\r\n+CIPSTATUS:0,\"TCP\",\"192.168.100.7\",9786,9988,0");
//    Serial1.print(pattern_OK);
//
//    Serial.print("STATUS:2\r\n+CIPSTATUS:0,\"TCP\",\"192.168.100.7\",9786,9988,0");
//    Serial.print(pattern_OK);

    return true;
}


static bool cipmux_setup(char msg[])
{
//    MatchState regx;
//
//    msg.toCharArray(buffer, sizeof(buffer));
//    regx.Target (buffer);
//
//    char result = regx.Match ("%a+([%?%=])(%d*)%c+", 0);
//    if (result == REGEXP_MATCHED)
//    {
//        mux = (TCPIP_Mux_t)(*(regx.GetCapture (buffer, 1)));
//    }
//
//    if(mux != MUX_SINGLE && mux != MUX_MULTIPLE)
//    {
//        transmit_error();
//        return false;
//    }
//
//    transmit_ok();
    return true;
}

static bool cipmux_request(char msg[])
{
//    Serial1.print(pattern_CIPMUX_Resp + (char)mux + "\r\n");
//    Serial1.print(pattern_OK);
//
//    Serial.print(pattern_CIPMUX_Resp + (char)mux + "\r\n");
//    Serial.print(pattern_OK);

    return true;
}

static bool cipmode_setup(char msg[])
{
//    MatchState regx;
//
//    msg.toCharArray(buffer, sizeof(buffer));
//    regx.Target (buffer);
//
//    char result = regx.Match ("%a+([%?%=])(%d*)%c+", 0);
//    if (result == REGEXP_MATCHED)
//    {
//        mode = (TCPIP_Mode_t)(*(regx.GetCapture (buffer, 1)));
//    }
//
//    if(mode != NORMAL_MODE && mode != TRANSPARENT_MODE)
//    {
//        transmit_error();
//        return false;
//    }
//
//    transmit_ok();
    return true;
}

static bool cipmode_request(char msg[])
{
//    Serial1.print(pattern_CIPMUX_Resp + (char)mode + "\r\n");
//    Serial1.print(pattern_OK);
//
//    Serial.print(pattern_CIPMUX_Resp + (char)mode + "\r\n");
//    Serial.print(pattern_OK);

    return true;
}



static bool cipstart_request(char msg[])
{
//    static uint8_t state = 0;
//
//    switch(state)
//    {
//        case 0:
//            Serial1.print(pattern_CIPSTART_Resp + String("\"TCP\",") + "\"" + remoteIP + "\"," + remotePort + "\r\n");
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_CIPSTART_Resp + String("\"TCP\",") + "\"" + remoteIP + "\"," + remotePort + "\r\n");
//            Serial.print(pattern_OK);
//            state++;
//            break;
//        case 1:
//            Serial1.print(pattern_CIPSTART_Resp + String("\"UDP\",") + "\"" + remoteIP + "\"," + remotePort + "," + localPortUDP + "," + modeUDP + "\r\n");
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_CIPSTART_Resp + String("\"UDP\",") + "\"" + remoteIP + "\"," + remotePort + "," + localPortUDP + "," + modeUDP + "\r\n");
//            Serial.print(pattern_OK);
//            state++;
//            break;
//        case 2:
//            Serial1.print(pattern_CIPSTART_Resp + linkID + "," + String("\"TCP\",") + "\"" + remoteIP + "\"," + remotePort + "\r\n");
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_CIPSTART_Resp + linkID + "," + String("\"TCP\",") + "\"" + remoteIP + "\"," + remotePort + "\r\n");
//            Serial.print(pattern_OK);
//            state = 0;
//            break;
//    }

    return true;
}


static bool cipstart_setup(char msg[])
{
//    static uint8_t state = 0;
//    MatchState regx;
//    char* ptr = NULL;
//    String _ip = "";
//    String Station_ip = "";
//    char result = 0;
//    String _type = "";
//    String _linkID = "";
//    String _remotePort = "";
//    String _keepAlive = "";
//    String _localUDPPort = "";
//    String _modeUDP = "";
//
//    uint8_t typeOffset = 255;
//    uint8_t remoteIP1Offset = 255;
//    uint8_t remoteIP2Offset = 255;
//    uint8_t remoteIP3Offset = 255;
//    uint8_t remoteIP4Offset = 255;
//    uint8_t remotePortOffset = 255;
//    uint8_t linkIDOffset = 255;
//    uint8_t keepAliveOffset = 255;
//    uint8_t localUDPPortOffset = 255;
//    uint8_t modeUDPOffset = 255;
//
//    msg.toCharArray(buffer, sizeof(buffer));
//    regx.Target (buffer);
//
//    switch(state)
//    {
//        case 0:
//            result = regx.Match ("AT%+CIPSTART%=\"(%w+)\",\"(%d+).(%d+).(%d+).(%d+)\",(%d+)%c+", 0);
//            if (result != REGEXP_MATCHED)
//            {
//                Serial1.println(pattern_ERROR);
//                Serial.println(pattern_ERROR);
//                return false;
//            }
//
//            typeOffset = 0;
//            remoteIP1Offset = 1;
//            remoteIP2Offset = 2;
//            remoteIP3Offset = 3;
//            remoteIP4Offset = 4;
//            remotePortOffset = 5;
//            linkIDOffset = 255;
//            keepAliveOffset = 255;
//            localUDPPortOffset = 255;
//            modeUDPOffset = 255;
//
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_OK);
//
//            state++;
//            break;
//        case 1:
//            result = regx.Match ("AT%+CIPSTART%=(%d+),\"(%w+)\",\"(%d+).(%d+).(%d+).(%d+)\",(%d+)%c+", 0);
//            if (result != REGEXP_MATCHED)
//            {
//                Serial1.println(pattern_ERROR);
//                Serial.println(pattern_ERROR);
//                return false;
//            }
//
//            typeOffset = 1;
//            remoteIP1Offset = 2;
//            remoteIP2Offset = 3;
//            remoteIP3Offset = 4;
//            remoteIP4Offset = 5;
//            remotePortOffset = 6;
//            linkIDOffset = 0;
//            keepAliveOffset = 255;
//            localUDPPortOffset = 255;
//            modeUDPOffset = 255;
//
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_OK);
//
//            state++;
//            break;
//        case 2:
//            result = regx.Match ("AT%+CIPSTART%=\"(%w+)\",\"(%d+).(%d+).(%d+).(%d+)\",(%d+),(%d+)%c+", 0);
//            if (result != REGEXP_MATCHED)
//            {
//                Serial1.println(pattern_ERROR);
//                Serial.println(pattern_ERROR);
//                return false;
//            }
//
//            typeOffset = 0;
//            remoteIP1Offset = 1;
//            remoteIP2Offset = 2;
//            remoteIP3Offset = 3;
//            remoteIP4Offset = 4;
//            remotePortOffset = 5;
//            linkIDOffset = 255;
//            keepAliveOffset = 6;
//            localUDPPortOffset = 255;
//            modeUDPOffset = 255;
//
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_OK);
//
//            state++;
//            break;
//        case 3:
//            result = regx.Match ("AT%+CIPSTART%=(%d+),\"(%w+)\",\"(%d+).(%d+).(%d+).(%d+)\",(%d+),(%d+)%c+", 0);
//            if (result != REGEXP_MATCHED)
//            {
//                Serial1.println(pattern_ERROR);
//                Serial.println(pattern_ERROR);
//                return false;
//            }
//
//            typeOffset = 1;
//            remoteIP1Offset = 2;
//            remoteIP2Offset = 3;
//            remoteIP3Offset = 4;
//            remoteIP4Offset = 5;
//            remotePortOffset = 6;
//            linkIDOffset = 0;
//            keepAliveOffset = 7;
//            localUDPPortOffset = 255;
//            modeUDPOffset = 255;
//
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_OK);
//
//            state++;
//            break;
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//        case 4:
//            result = regx.Match ("AT%+CIPSTART%=\"(%w+)\",\"(%d+).(%d+).(%d+).(%d+)\",(%d+)%c+", 0);
//            if (result != REGEXP_MATCHED)
//            {
//                Serial1.println(pattern_ERROR);
//                Serial.println(pattern_ERROR);
//                return false;
//            }
//
//            typeOffset = 0;
//            remoteIP1Offset = 1;
//            remoteIP2Offset = 2;
//            remoteIP3Offset = 3;
//            remoteIP4Offset = 4;
//            remotePortOffset = 5;
//            linkIDOffset = 255;
//            keepAliveOffset = 255;
//            localUDPPortOffset = 255;
//            modeUDPOffset = 255;
//
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_OK);
//
//            state++;
//            break;
//
//        case 5:
//            result = regx.Match ("AT%+CIPSTART%=(%d+),\"(%w+)\",\"(%d+).(%d+).(%d+).(%d+)\",(%d+)%c+", 0);
//            if (result != REGEXP_MATCHED)
//            {
//                Serial1.println(pattern_ERROR);
//                Serial.println(pattern_ERROR);
//                return false;
//            }
//
//            typeOffset = 1;
//            remoteIP1Offset = 2;
//            remoteIP2Offset = 3;
//            remoteIP3Offset = 4;
//            remoteIP4Offset = 5;
//            remotePortOffset = 6;
//            linkIDOffset = 0;
//            keepAliveOffset = 255;
//            localUDPPortOffset = 255;
//            modeUDPOffset = 255;
//
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_OK);
//
//            state++;
//            break;
//        case 6:
//            result = regx.Match ("AT%+CIPSTART%=\"(%w+)\",\"(%d+).(%d+).(%d+).(%d+)\",(%d+),(%d+),(%d+)%c+", 0);
//            if (result != REGEXP_MATCHED)
//            {
//                Serial1.println(pattern_ERROR);
//                Serial.println(pattern_ERROR);
//                return false;
//            }
//
//            typeOffset = 0;
//            remoteIP1Offset = 1;
//            remoteIP2Offset = 2;
//            remoteIP3Offset = 3;
//            remoteIP4Offset = 4;
//            remotePortOffset = 5;
//            linkIDOffset = 255;
//            keepAliveOffset = 255;
//            localUDPPortOffset = 6;
//            modeUDPOffset = 7;
//
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_OK);
//
//            state++;
//            break;
//        case 7:
//            result = regx.Match ("AT%+CIPSTART%=(%d+),\"(%w+)\",\"(%d+).(%d+).(%d+).(%d+)\",(%d+),(%d+),(%d+)%c+", 0);
//            if (result != REGEXP_MATCHED)
//            {
//                Serial1.println(pattern_ERROR);
//                Serial.println(pattern_ERROR);
//                return false;
//            }
//
//            typeOffset = 1;
//            remoteIP1Offset = 2;
//            remoteIP2Offset = 3;
//            remoteIP3Offset = 4;
//            remoteIP4Offset = 5;
//            remotePortOffset = 6;
//            linkIDOffset = 0;
//            keepAliveOffset = 255;
//            localUDPPortOffset = 7;
//            modeUDPOffset = 8;
//
//            Serial1.print(pattern_OK);
//            Serial.print(pattern_OK);
//
//            state++;
//            break;
//
//        case 8:
//            Serial1.print("ALREADY	CONNECTED\r\n");
//            Serial.print("ALREADY	CONNECTED\r\n");
//
//            state++;
//            break;
//        case 9:
//            Serial1.print(pattern_ERROR);
//            Serial.print(pattern_ERROR);
//
//            state = 0;
//            break;
//    }
//
//    if(linkIDOffset != 255)
//    {
//        ptr = regx.GetCapture (buffer, linkIDOffset);
//        _linkID = String(ptr);
//        linkID = _linkID;
//    }
//    if(typeOffset != 255)
//    {
//        ptr = regx.GetCapture (buffer, typeOffset);
//        _type = String(ptr);
//        type = _type;
//    }
//    if(remoteIP1Offset != 255)
//    {
//        remoteIP = "";
//        ptr = regx.GetCapture (buffer, remoteIP1Offset);
//        _ip = String(ptr);
//        remoteIP += _ip + ".";
//
//        ptr = regx.GetCapture (buffer, remoteIP2Offset);
//        _ip = String(ptr);
//        remoteIP += _ip + ".";
//
//        ptr = regx.GetCapture (buffer, remoteIP3Offset);
//        _ip = String(ptr);
//        remoteIP += _ip + ".";
//
//        ptr = regx.GetCapture (buffer, remoteIP4Offset);
//        _ip = String(ptr);
//        remoteIP += _ip;
//    }
//    if(remotePortOffset != 255)
//    {
//        ptr = regx.GetCapture (buffer, remotePortOffset);
//        _remotePort = String(ptr);
//        remotePort = _remotePort;
//    }
//
//    if(keepAliveOffset != 255)
//    {
//        ptr = regx.GetCapture (buffer, keepAliveOffset);
//        _keepAlive = String(ptr);
//        keepAlive = _keepAlive;
//    }
//
//    if(localUDPPortOffset != 255)
//    {
//        ptr = regx.GetCapture (buffer, localUDPPortOffset);
//        _localUDPPort = String(ptr);
//    }
//
//    if(modeUDPOffset != 255)
//    {
//        ptr = regx.GetCapture (buffer, modeUDPOffset);
//        _modeUDP = String(ptr);
//    }

    return true;
}
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
 * @brief	Function handle of all at wi-fi commands.
 * @param   msg[in]	- Received at message.
 * @return  true/false.
**/
bool esp_tcp_handle(char msg[])
{  
    if(strstr(msg, pattern_CIPSTATUS_Cmd))
    {
        cipstatus_request(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPMUX_Query))
    {
        cipmux_request(msg);
        return true;  
    }
    if(strstr(msg, pattern_CIPMUX_Cmd))
    {
        cipmux_setup(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPMODE_Query))
    {
        cipmode_request(msg);
        return true;  
    }
    if(strstr(msg, pattern_CIPMODE_Cmd))
    {
        cipmode_setup(msg);
        return true;  
    }



    if(strstr(msg, pattern_CIPSTART_Query))
    {
        cipstart_request(msg);
        return true;  
    }

    if(strstr(msg, pattern_CIPSTART_Cmd))
    {
        cipstart_setup(msg);
        return true;  
    }

    transmit_error();
    return false;
}
