#ifndef __OBSMSG_H
#define __OBSMSG_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define OBS_BUFFER_SIZE   		60
#define OBS_USART_LENGTH     	23  
#define OBS_REMOTE_USART_LENGTH 28   
#define OBS_CMD_USART_LENGTH 	13
#define OBS_USART_MAX        	(OBS_USART_LENGTH+2)
#define OBS_REMOTE_USART_MAX    (OBS_REMOTE_USART_LENGTH+2)
#define OBS_CMD_USART_MAX    	(OBS_CMD_USART_LENGTH+2)
#define OBS_USART_MAX_LENGTH    ((OBS_REMOTE_USART_LENGTH>OBS_USART_LENGTH)?OBS_REMOTE_USART_LENGTH:OBS_USART_LENGTH)

#define OBS_RCON_L_OFFSET           ((uint8_t)8)
#define OBS_RCON_FL_OFFSET          ((uint8_t)14)
#define OBS_RCON_FR_OFFSET          ((uint8_t)20)
#define OBS_RCON_R_OFFSET           ((uint8_t)26)

typedef void (*ObsMsg_SetTempRemote)(uint8_t code);
typedef void (*ObsMsg_SetTempRcon)(uint64_t temp_code,uint8_t remote);
typedef void (*ObsMsg_UsartSend)(uint16_t length,char *data);
typedef void(*ObsMsg_SetPowerState)(uint8_t state);

typedef enum
{
    OBSMSG_DATA_WALL = 1,
    OBSMSG_DATA_RCON = 2,
    OBSMSG_DATA_CMD = 3,
}ObsMsgData_t;

typedef enum
{
    OBSMSG_IDLE = 0,
    OBSMSG_ENABLE = 1,
    OBSMSG_DISABLE = 2,
    OBSMSG_SLEEP = 3,
    OBSMSG_RESTART = 4,
    OBSMSG_VERSION = 5,
}ObsMsgCommand_t;

typedef enum
{
    OBSMSG_RECIEVE_REMOTE = 1,
    OBSMSG_RECIEVE_VIRTUAL= 2,
    OBSMSG_RECIEVE_CHARGE = 3,
}ObsMsgRecieve_t;

typedef enum 
{
    OBSMSG_RCONSIDE_NONE = 0,
    OBSMSG_RCONSIDE_LEFT,
    OBSMSG_RCONSIDE_RIGHT,
}ObsMsgRconSide_t;

typedef struct
{
    volatile uint8_t rxbuffer[OBS_BUFFER_SIZE];
    uint16_t version;
    uint8_t update;
    uint8_t length;
    uint8_t ack;
    uint16_t rx_fps;
    uint16_t real_fps;
    uint8_t cur_mode;
    uint8_t buffer_idx;
    uint8_t tx_buffer[20];
    uint32_t check_time;   
    volatile uint16_t *p_obs_adc_left;
    volatile uint16_t *p_obs_adc_front;
    volatile uint16_t *p_obs_adc_right;
    volatile uint16_t *p_wall_adc_left;
    volatile uint16_t *p_wall_adc_right;
    volatile uint16_t *p_cliff_adc_left;
    volatile uint16_t *p_cliff_adc_front;
    volatile uint16_t *p_cliff_adc_right;
    ObsMsg_SetTempRcon  on_set_temp_rcon;
    ObsMsg_SetTempRemote  on_set_temp_remote;
    ObsMsg_UsartSend on_usart_send;
    ObsMsg_SetPowerState on_set_power_state;
}ObsMsg_t;

extern ObsMsg_t g_obsmsg;




void ObsMsg_Init(void);
uint8_t ObsMsg_Parse(volatile uint8_t *msg);
uint8_t ObsMsg_IsAck(uint8_t ack);
uint8_t ObsMsg_CheckAckFail(uint8_t cmd);
void ObsMsg_Cmd(uint8_t cmd);
void ObsMsg_LoadCmdData(volatile uint8_t *p_buffer,uint8_t cmd);
uint8_t ObsMsg_CheckAckFail(uint8_t cmd);
uint8_t ObsMsg_SetMode(uint8_t cmd);
uint8_t ObsMsg_IsIdle(void);
#endif















