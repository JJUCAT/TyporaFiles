#ifndef __REMOTE_H
#define __REMOTE_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


typedef enum
{
	REMOTE_SIGNAL_NONE					= 0,																				  
	REMOTE_SIGNAL_NAV  					= 0x00000001,																				
	REMOTE_SIGNAL_FORWARD				= 0x00000002,																				
	REMOTE_SIGNAL_LEFT					= 0x00000004,																																							
	REMOTE_SIGNAL_BACK					= 0x00000008,																				
	REMOTE_SIGNAL_RIGHT					= 0x00000010,																																								
	REMOTE_SIGNAL_WATER					= 0x00000020,																				
	REMOTE_SIGNAL_AREA					= 0x00000040,																				
	REMOTE_SIGNAL_SPEAKER				= 0x00000080,																				
	REMOTE_SIGNAL_SPOT					= 0x00000100,
	REMOTE_SIGNAL_MAINBRUSH				= 0x00000200,
	REMOTE_SIGNAL_CLEAN_CNT				= 0x00000400,	
	REMOTE_SIGNAL_WIFI_STOP				= 0x00000800,
	REMOTE_SIGNAL_GO_HOME				= 0x00001000,
	REMOTE_SIGNAL_WIFI_DIR				= 0x00002000,
	REMOTE_SIGNAL_WIFI_FORWARD			= 0x00004000,
	REMOTE_SIGNAL_WIFI_BACK				= 0x00008000,
	REMOTE_SIGNAL_WIFI_LEFT				= 0x00010000,
	REMOTE_SIGNAL_WIFI_RIGHT			= 0x00020000,
	REMOTE_SIGNAL_WIFI_PAUSE			= 0x00040000,	
	REMOTE_SIGNAL_WIFI_NAV				= 0x00080000,
	REMOTE_SIGNAL_WALL					= 0x00100000,	
}RemoteSignal_t;

typedef enum
{
	REMOTE_CODE_NONE			= 	0,																				  
	REMOTE_CODE_NAV				=	0X2A,																				
	REMOTE_CODE_FORWARD			=	0x1B,																				
	REMOTE_CODE_LEFT			=	0X3B,																				
	REMOTE_CODE_BACK			=	0X4C,																				
	REMOTE_CODE_RIGHT			=	0X46,																																								
	REMOTE_CODE_WATER_SPOT		=	0x09,//w410:water flow w400:spot																				
	REMOTE_CODE_AREA			=	0X55,																				
	REMOTE_CODE_SPEAKER_WALL	= 	0X6E,//w410:speaker w400:wall 																				
	REMOTE_CODE_SPOT_HOME		=	0X5F,//w410:spot    w400:home
	REMOTE_CODE_MAINBRUSH		=	0X69,
	REMOTE_CODE_CLEAN_CNT		=	0X23,	
}RemoteCode_t;

typedef struct 
{
	RemoteSignal_t real;
	RemoteSignal_t temp;
	RemoteSignal_t pre_temp;
	uint8_t is_new;
	uint32_t recieve_time;
}Remote_t;

extern Remote_t g_remote;
void Remote_Init(void);
void Remote_SetTempRemote(RemoteSignal_t remote);
void Remote_ResetTempRemote(void);
void Remote_SetTempRemoteWithCode(uint8_t code);
void Remote_FilterProcess(void);
uint8_t Remote_IsRemoteEvent(RemoteSignal_t remote);
uint8_t Remote_IsRemoteExist(RemoteSignal_t remote);
void Remote_Reset(void);


#endif



