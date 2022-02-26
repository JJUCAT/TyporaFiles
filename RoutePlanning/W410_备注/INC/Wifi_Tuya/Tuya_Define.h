#ifndef __TUYA_DEFINE_H
#define __TUYA_DEFINE_H

#include "stdint.h"

#define         TX_BUF_LMT         96
#define         RECV_BUF_LMT       32
#define         CONFIG_MODE_DEFAULT             "0"                             //Default mode
#define         CONFIG_MODE_LOWPOWER            "1"                             //low power mode
#define         CONFIG_MODE_SPECIAL             "2"                             //special mode
#define         DATA_PROCESS_LMT                24
#define         TY_REALMAP_SIZE_LIMIT          100  
#define         TY_REALMAP_BUSY_SIZE_LIMIT     20  
#define         TY_HISTORY_MAP_SIZE_LIMIT      100  
#define         MAP_HISTORY_MAP_MAX            100 
#define         SUPPORT_MCU_RTC_CHECK           1

#define PRODUCT_KEY "jrpug38exzg9g1ha"//"idwa6b1rrcmtyytw"//"xqstbvfxs0hpg118"//"ixlrgw4nrlj0ozzt"//"b0awvuuhfc590xrm"//"idwa6b1rrcmtyytw"//"ixisj64x1wmfsxil"
#define MCU_VER "1.0.0" 
#define CONFIG_MODE     CONFIG_MODE_SPECIAL 

#define VERSION                 0x00                                            //Module send frame protocol version number
#define MCU_SEND_VER            0x03                                            //MCU send frame protocol version number(default)
#define PROTOCOL_HEAD           0x07                                            //Fixed protocol header length
#define FRAME_FIRST             0x55                                            //Frame header first byte
#define FRAME_SECOND            0xaa                                            //Frame header second byte

#define HEAD_FIRST                      0
#define HEAD_SECOND                     1        
#define PROTOCOL_VERSION                2
#define FRAME_TYPE                      3
#define LENGTH_HIGH                     4
#define LENGTH_LOW                      5
#define DATA_START                      6

#define         HEAT_BEAT_CMD                   0                               //Heartbeat package
#define         PRODUCT_INFO_CMD                1                               //Product information
#define         WORK_MODE_CMD                   2                               //Query the module working mode set by the MCU
#define         WIFI_STATE_CMD                  3                               //Wifi working status
#define         WIFI_RESET_CMD                  4                               //Reset wifi
#define         WIFI_MODE_CMD                   5                               //Select smartconfig/AP mode
#define         DATA_QUERT_CMD                  6                               //Order send
#define         STATE_UPLOAD_CMD                7                               //Status upload	 
#define         STATE_QUERY_CMD                 8                               //Status query   
#define         UPDATE_START_CMD                0x0a                            //Upgrade start
#define         UPDATE_TRANS_CMD                0x0b                            //Upgrade transfer
#define         GET_ONLINE_TIME_CMD             0x0c                            //Get system time (Greenwich Mean Time)
#define         FACTORY_MODE_CMD                0x0d                            //Enter production test mode    
#define         WIFI_TEST_CMD                   0x0e                            //Wifi function test
#define         GET_LOCAL_TIME_CMD              0x1c                            //Get local time
#define         WEATHER_OPEN_CMD                0x20                            //Turn on the weather          
#define         WEATHER_DATA_CMD                0x21                            //Weather data
#define         STATE_UPLOAD_SYN_CMD            0x22                            //Status upload (synchronization)
#define         STATE_UPLOAD_SYN_RECV_CMD       0x23                            //Status upload results(synchronization)
#define         HEAT_BEAT_STOP                  0x25                            //Turn off the WIFI module heartbeat
#define         STREAM_START_CMD                0x26                            //start stream
#define         STREAM_NEW_ID_CMD               0x27                            //new id stream
#define         STREAM_TRANS_CMD                0x28                            //Stream data transmission
#define         STREAM_END_CMD                  0x29                            //end stream
#define         GET_WIFI_STATUS_CMD             0x2b                            //Gets the wifi networking status
#define         WIFI_CONNECT_TEST_CMD           0x2c                            //Wifi function test(connection designated route)
#define         GET_MAC_CMD                     0x2d                            //Get module mac
#define         GET_IR_STATUS_CMD               0x2e                            //IR status notification
#define         IR_TX_RX_TEST_CMD               0x2f                            //IR into send-receive test
#define         MAPS_STREAM_TRANS_CMD           0x30                            //streams trans(Support for multiple maps)
#define         FILE_DOWNLOAD_START_CMD         0x31                            //File download startup
#define         FILE_DOWNLOAD_TRANS_CMD         0x32                            //File download data transfer
#define         OPEN_MODULE_TIME_SERVE_CMD      0x34                            //Open the module time service notification
#define         BLE_TEST_CMD                    0x35                            //Bluetooth functional test（Scan designated bluetooth beacon）
#define         GET_VOICE_STATE_CMD             0x60                            //Gets the voice status code
#define         MIC_SILENCE_CMD                 0x61                            //MIC mute Settings
#define         SET_SPEAKER_VOLUME_CMD          0x62                            //speaker volume set
#define         VOICE_TEST_CMD                  0x63                            //Audio production test
#define         VOICE_AWAKEN_TEST_CMD           0x64                            //Wake up production test


#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_CONN_CLOUD                 0x04
#define         WIFI_LOW_POWER                  0x05
#define         SMART_AND_AP_STATE              0x06
#define         WIFI_SATE_UNKNOW                0xff

#define         DP_TYPE_RAW                     0x00        //RAW type
#define         DP_TYPE_BOOL                    0x01        //bool type
#define         DP_TYPE_VALUE                   0x02        //value type
#define         DP_TYPE_STRING                  0x03        //string type
#define         DP_TYPE_ENUM                    0x04        //enum type
#define         DP_TYPE_FAULT                   0x05        //fault type




#define DPID_POWER 1
#define DPID_SWITCH_GO 2
#define DPID_MODE 5
#define DPID_ELECTRICITY_LEFT 12
#define DPID_CLEANAREA 14
#define DPID_STATUS 15
#define DPID_FAULT 17
#define DPID_WATER_CONTROL 20
#define DPID_DIRECTIONCONTROL 101
#define DPID_CLEAN_RECORD 102
#define DPID_EDGEBRUSH 108
#define DPID_ROLLBRUSH 109
#define DPID_FILTER 110
#define DPID_RESETFILTER 111
#define DPID_RESETROLLBRUSH 112
#define DPID_RESETEDGEBRUSH 113
#define DPID_RESUME 114
#define DPID_MAX 115
#define DPID_SEEK 116
#define DPID_CLEARTIME 119
#define DPID_TONESWITCH 120
#define DPID_TONEVOLUME 121
#define DPID_WATERLEVELCONTROL 122

#define         TRUE                1
#define         FALSE               0

#define         SUCCESS             1
#define         ERROR               0

#define         RESET_WIFI_ERROR                0
#define         RESET_WIFI_SUCCESS              1

#define         SET_WIFICONFIG_ERROR            0
#define         SET_WIFICONFIG_SUCCESS          1

#define         SMART_CONFIG                    0x0  
#define         AP_CONFIG                       0x1   
#define         NO_CONFIG                       0x2

#define         WIFI_STREAM_ENABLE              1


#define MAP_STREAM_STATE_WAIT_RETURN 0XFF

#define MAP_STREAM_STATE_START_SUCCESS 0X00

#define MAP_STREAM_STATE_START_FAIL 0X01

#define MAP_STREAM_STATE_NEW_ID_SUCCESS 0X00

#define MAP_STREAM_STATE_NEW_ID_FUNCTIN_NOT_START 0X01

#define MAP_STREAM_STATE_NEW_ID_CONNECT_FAIL 0X02

#define MAP_STREAM_STATE_NEW_ID_OVER_TIME 0X03

#define MAP_STREAM_STATE_DATA_SUCCESS 0X00

#define MAP_STREAM_STATE_DATA_FUNCTIN_NOT_START 0X01

#define MAP_STREAM_STATE_DATA_CONNECT_FAIL 0X02

#define MAP_STREAM_STATE_DATA_OVER_TIME 0X03

#define MAP_STREAM_STATE_DATA_LENGTH_ERROR 0X04

#define MAP_STREAM_STATE_END_SUCCESS 0X00

#define MAP_STREAM_STATE_END_FUNCTIN_NOT_START 0X01

#define MAP_STREAM_STATE_END_CONNECT_FAIL 0X02

#define MAP_STREAM_STATE_END_OVER_TIME 0X03

#define TUYA_REALMAP_RATE 165//(25/10)

#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_CONN_CLOUD                 0x04
#define         WIFI_LOW_POWER                  0x05
#define         SMART_AND_AP_STATE              0x06
#define         WIFI_SATE_UNKNOW                0xff

#define SUPPORT_GREEN_TIME 1



typedef struct
{
  uint32_t send_time;
  unsigned long offset;
  unsigned short id; 
  unsigned char state;  
  unsigned char set_state;
  unsigned char start_return_state; 
  unsigned char new_id_return_state; 
  unsigned char trans_return_state; 
  unsigned char end_return_state;      
  unsigned char new_start; 
  uint32_t start_time;
}TuyaMapStreamData_t;

typedef struct
{
  unsigned char dp_id;                              
  unsigned char dp_type;                            
}DataPoint_t;

typedef enum
{
  TY_CELL_STATE_CURRENT= 0,
  TY_CELL_STATE_BLOCK,
  TY_CELL_STATE_CLEAN,
  TY_CELL_STATE_CHARGE,
  TY_CELL_STATE_OTHER,
}TuyaCellState_t;

typedef struct
{
	Point16_t cell;
	TuyaCellState_t status;
}TuyaReportCell_t;

typedef struct
{
	uint8_t report_switch;
	uint32_t start_time;   
	uint32_t cleaned_area; 
	uint16_t cleaned_time;
  Point16_t robot_cell; 
} TuyaRealMapStruct_t;

typedef struct
{
  uint8_t get_time_state;
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint8_t second;
}TuyaTime_t;

typedef struct
{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t min;
	uint32_t cleaned_area; 
	uint16_t cleaned_time;
  uint16_t id;
  uint8_t start_time_valid;
}TuyaHistory_t;

typedef struct
{
    TuyaHistory_t history;
    TuyaRealMapStruct_t realtime_map;
    TuyaMapStreamData_t map_stream;
    TuyaTime_t time;
    unsigned char wifi_set_work_state;
    unsigned char wifi_cur_work_state;
    unsigned char wifi_set_mode;
    unsigned char wifi_reset_state;
    unsigned char wifi_stop_update;
    unsigned char set_workmode;
    unsigned char cur_workmode;
    unsigned char cur_workstatus;
    unsigned char clean_mode;
    unsigned char pause_mode;
    unsigned char battery_percent;
    unsigned char errorcode;
    unsigned char find_robot_cnt;
    unsigned char speaker_enable;
    char data_buf[PROTOCOL_HEAD + DATA_PROCESS_LMT];
    uint8_t realmap_cnt;
    uint8_t realmap_input;
    uint8_t realmap_output;
    uint8_t realmap_busy;
    Point16_t realmap_list[TY_REALMAP_SIZE_LIMIT+1];
    Point16_t realmap_busy_list[TY_REALMAP_BUSY_SIZE_LIMIT+1];
    uint8_t realmap_busy_cnt;
    uint8_t realmap_busy_input;
    uint8_t realmap_busy_output;
    uint8_t history_buffer[24];
    char rx_buf[PROTOCOL_HEAD + RECV_BUF_LMT];
    char *p_rx_queue_in;
    char *p_rx_queue_out;   
    char tx_buf[PROTOCOL_HEAD + TX_BUF_LMT]; 
    uint8_t is_working;
    uint8_t is_wifi_connected;
    uint16_t pump_cycle_cnt;
    uint8_t water_flow;
}TuyaStruct_t;

typedef enum
{
  TY_DL_WM_SLEEP = 0,
  TY_DL_WM_SPOT = 1,  
  TY_DL_WM_SMART = 2,
  TY_DL_WM_CHARGE_GO = 3, 
  TY_DL_WM_WALLFOLLOW = 4,   
  TY_DL_WM_PAUSE = 5,  
  TY_DL_WM_ERROR = 0XFF,  
}TuyaDownLoadWorkMode_t;

typedef enum
{
  TY_UL_WM_STANBY = 0,
  TY_UL_WM_SPOT = 1,  
  TY_UL_WM_SMART = 2,
  TY_UL_WM_CHARGE_GO = 3, 
  TY_UL_WM_WALLFOLLOW = 4,   
  TY_UL_WM_SLEEP = 5,
  TY_UL_WM_CHARGELINE = 6,
  TY_UL_WM_BASE = 7,
  TY_UL_WM_REMOTE = 8,
  TY_UL_WM_LOWPOWER = 9,
  TY_UL_WM_PAUSE = 10,    
}TuyaUpLoadWorkMode_t;

typedef enum
{
  TY_MAP_STREAM_IDLE = 0,
  TY_MAP_STREAM_WAIT = 1,
  TY_MAP_STREAM_START = 2,
  TY_MAP_STREAM_CREATE_NEW_ID = 3,
  TY_MAP_STREAM_DATA = 4,
  TY_MAP_STREAM_END = 5, 
}TuyaMapDataStream_t;

typedef enum
{
  TY_ERROR_CODE_NONE = 0,
  TY_ERROR_CODE_BUMPER,
  TY_ERROR_CODE_OBS,
  TY_ERROR_CODE_WALL,
  TY_ERROR_CODE_CLIFF,
  TY_ERROR_CODE_PICKUP,
  TY_ERROR_CODE_MOBILITY,
  TY_ERROR_CODE_LSIDE_BRUSH,
  TY_ERROR_CODE_RSIDE_BRUSH,
  TY_ERROR_CODE_SIDE_BRUSH,
  TY_ERROR_CODE_L_WHEEL,
  TY_ERROR_CODE_R_WHEEL,
  TY_ERROR_CODE_M_BRUSH,
  TY_ERROR_CODE_FAN,
  TY_ERROR_CODE_PUMP,
  TY_ERROR_CODE_DUSTBIN,
  TY_ERROR_CODE_WATERTANK,
  TY_ERROR_CODE_FILTER,

  TY_ERROR_CODE_GYRO = 20,
  TY_ERROR_CODE_LIDA,
  TY_ERROR_CODE_CAMERA,
  TY_ERROR_CODE_STUCK,
  TY_ERROR_CODE_BAT = 30,
  TY_ERROR_LOW_BATTERY  = 31,
}TuyaErrorCode_t;







#endif







