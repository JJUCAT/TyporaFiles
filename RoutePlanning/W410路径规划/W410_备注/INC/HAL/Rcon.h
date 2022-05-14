#ifndef __RCON_H
#define __RCON_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define RCON_STANDRD_HEAD_LOW_MIN       40   
#define RCON_STANDRD_HEAD_LOW_MAX       50

#define RCON_HEAD_LOW_MIN               80
#define RCON_HEAD_LOW_MAX               100
#define RCON_HEAD_HIGH_MIN              40   
#define RCON_HEAD_HIGH_MAX              50

#define RCON_LOGIC_1_MIN                14   
#define RCON_LOGIC_1_MAX                21
#define RCON_LOGIC_0_MIN                4    
#define RCON_LOGIC_0_MAX                9
#define RCON_LOW_MIN                    3
#define RCON_LOW_MAX                    9


#define RCON_CHARGE_LEFT     	(uint8_t)0x50
#define RCON_CHARGE_RIGHT    	(uint8_t)0x02
#define RCON_CHARGE_TOP      	(uint8_t)0x0c

#define RCON_W_CHARGE_RIGHT    (uint8_t)0x37//0x6E
#define RCON_W_CHARGE_LEFT     (uint8_t)0x5B//0xB6 10110110
#define RCON_W_CHARGE_TOP	   (uint8_t)0x18//0x30

#define RCON_VW_NORMAL       	(uint8_t)0x24
#define RCON_VW_TOP          	(uint8_t)0x64

#define RCON_FL_VW_T	 		(uint32_t)0x00000001
#define RCON_FL_VW_N	 		(uint32_t)0x00000002
#define RCON_FR_VW_T	 		(uint32_t)0x00000004
#define RCON_FR_VW_N	 		(uint32_t)0x00000008	
#define RCON_FR_HOME_L        	(uint32_t)0x00000010
#define RCON_FR_HOME_R        	(uint32_t)0x00000020
#define RCON_FL_HOME_L        	(uint32_t)0x00000040
#define RCON_FL_HOME_R        	(uint32_t)0x00000080
#define RCON_FR_HOME_T        	(uint32_t)0x00000100
#define RCON_FL_HOME_T        	(uint32_t)0x00000200

typedef void (*Rcon_SetTempRemote)(uint8_t code);
typedef uint8_t (*Rcon_GetGpioState)(void);
typedef struct
{
	 uint8_t remote_flag;
	 uint16_t temp_counter;
	 uint64_t temp_code;
	 uint8_t receive_start;
	 uint8_t bit_counter;
	 uint16_t time_counter;
}Rcon_Element;

typedef enum 
{
    RCONSIDE_NONE = 0,
    RCONSIDE_FL,
    RCONSIDE_FR,
}RconSide_t;

typedef struct
{
	uint32_t status;
	Rcon_Element fl;
	Rcon_Element fr;
	Rcon_GetGpioState on_get_fl_state;
	Rcon_GetGpioState on_get_fr_state;
	Rcon_SetTempRemote  on_set_temp_remote;
}Rcon_t;

extern Rcon_t g_rcon;

void Rcon_Init(void);
uint32_t Rcon_GetStatus(void);
void Rcon_ResetStatus(void);
void Rcon_ResetTheStatus(uint32_t status);
void Rcon_IncreaseCounter(void);
void Rcon_ClearState(volatile Rcon_Element *rcon);
void Rcon_ResetTempCode(Rcon_Element *rcon);
void Rcon_SetState(uint64_t temp_code,uint8_t remote);
void Rcon_Decoder(Rcon_Element *element,uint8_t state,RconSide_t side);



#endif



















