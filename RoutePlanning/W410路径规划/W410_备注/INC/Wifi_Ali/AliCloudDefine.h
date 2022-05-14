#ifndef __ALICLOUDDEFINE_H
#define __ALICLOUDDEFINE_H

#include <stdint.h>
#include "Hw_SysInitialize.h"
      
#define ALI_PROTOCOL_VER                0X00
#define ALI_REALMAP_SIZE     		    100  
#define ALI_REALMAP_BUSY_SIZE     		20  
#define ALI_HISTORY_MAP_SIZE		    100  
#define ALI_RX_BUF_TEMP_SIZE            100
#define ALI_RX_BUF_SIZE                 100
#define ALI_RX_BUF_PRE_SIZE             40
#define ALI_TRUE						1
#define ALI_FALSE						0
#define ALI_REALMAP_RATE			    91/100
#define ALI_TX_QUEUE_MAX    			100
#define ALI_TX_BUF_MAX      			600
#define ALI_REAL_MAP_MAX_CNT 			400

typedef enum
{
	ALI_ATTRID_BATTERYSTATE = 0,
	ALI_ATTRID_WORKMODE = 1,
	ALI_ATTRID_PAUSESWITCH = 2, 
	ALI_ATTRID_POWERSWITCH = 8,
	ALI_ATTRID_DIRECTION = 9,
	ALI_ATTRID_SCHEDU_1 = 10,
	ALI_ATTRID_SCHEDU_2 = 11,
	ALI_ATTRID_SCHEDU_3 = 12,
	ALI_ATTRID_SCHEDU_4 = 13,
	ALI_ATTRID_SCHEDU_5 = 14,
	ALI_ATTRID_SCHEDU_6 = 15,
	ALI_ATTRID_SCHEDU_7 = 16,
	ALI_ATTRID_MAXMODE = 17, 
	ALI_ATTRID_WATERTANKLEVEL = 18,
	ALI_ATTRID_LIFE = 19,
	ALI_ATTRID_BEEPVOLUME = 20,
	ALI_ATTRID_BEEPTYPE = 21,
	ALI_ATTRID_N0_DISTURB =22,
	ALI_ATTRID_FINDROBOT = 23,
	ALI_ATTRID_CARPETCONTROL = 24,
	ALI_ATTRID_CLEANTYPE = 25,
	ALI_ATTRID_REALMAPSTART = 26,
	ALI_ATTRID_REALMAP = 27,
	ALI_ATTRID_HISTORYSTARTTING= 28, 
	ALI_ATTRID_HISTORY = 29,
	ALI_ATTRID_CHARGER_POSITION = 30,
	ALI_ATTRID_VW_ENABLE = 31, //virturl wall enable
	ALI_ATTRID_VW_DATA = 32,
	ALI_ATTRID_PARTITIONDATA = 33,
	ALI_ATTRID_WIFI_INFO = 34,
	ALI_ATTRID_ROBOT_INFO = 35,
	ALI_ATTRID_OTA_INFO = 36,
	ALI_ATTRID_TIMEZONE = 37,
	ALI_ATTRID_RESFACTORY = 38,

	
	ALI_ATTRID_WATERCONTROL = 71,
	ALI_ATTRID_WHEELSPEED = 72,
	ALI_ATTRID_MAINBRUSHPOWER = 73,
	ALI_ATTRID_SIDRBRUSHPOWER = 74,
	ALI_ATTRID_FANPOWER = 75,
	ALI_ATTRID_CLEANLOOPCNT = 76,
	ALI_ATTRID_AREA_SET = 80,
	
	ALI_ATTRID_TIME_CALIBRATION = 115,
	ALI_ATTRID_WIFI_STATE = 200,
	ALI_ATTRID_UTC = 201,
	ALI_ATTRID_UNBANDING = 202,
	ALI_ATTRID_FACTORY_TEST = 203,
	ALI_ATTRID_WIFI_MAC = 204,

} AliAttrIdType_t;

typedef struct
{
	uint8_t method;
	uint32_t seq_num;
	uint8_t type_id;
	uint8_t event_id;
	AliAttrIdType_t attr_id;
	uint8_t data;
} AliTxMsgStruct_t;

typedef enum
{
	ALI_HEADER = 0xAA,
	ALI_VERSION = 0x01,
	ALI_CMD_TX = 0x00,
	ALI_CMD_RX = 0x80,
	ALI_METHOD_RX_GET= 0x00,
	ALI_METHOD_RX_SET = 0x01,
	ALI_METHOD_RX_SERIVER=0x02,	
	ALI_METHOD_REPORT_ATTR = 0x80,
	ALI_METHOD_REPORT_EVENT = 0x81,	
	ALI_METHOD_SYSTEM_GET = 0xF0,
	ALI_METHOD_SYSTEM_SET = 0xF1,	
	ALI_METHOD_OTA_TX_RX = 0xF2,
	ALI_METHOD_ACK = 0xFF,
} AliPakcet_t;

typedef enum
{
	ALI_EVENT_ERROR = 0,
} AliEventIdType_t;

typedef enum
{
	ALI_PROTOCOL_HEADER = 0x00,
	ALI_PROTOCOL_LENGHT_H = 0x01,
	ALI_PROTOCOL_LENGHT_L = 0x02,
	ALI_PROTOCOL_VERSION = 0x03,
	ALI_PROTOCOL_COMMAND = 0x04,
	ALI_PROTOCOL_METHOD = 0x05,
	ALI_PROTOCOL_SEQ_3 = 0x06,
	ALI_PROTOCOL_SEQ_2 = 0x07,
	ALI_PROTOCOL_SEQ_1 = 0x08,
	ALI_PROTOCOL_SEQ_0 = 0x09,	
	ALI_PROTOCOL_SET_TYPEID = 0x10,
	ALI_PROTOCOL_SET_ATTRID = 0x11,
	ALI_PROTOCOL_SET_VALUE  = 0x12,
	ALI_PROTOCOL_REPORT_TYPEID = 0x10,
	ALI_PROTOCOL_REPORT_ATTRID = 0x11,
	ALI_PROTOCOL_REPORT_VALUE = 0x12,	
	ALI_PROTOCOL_GET_ATTRID = 0x10,	
	ALI_PROTOCOL_ACK = 0x10,
} AliProtocol_t;

typedef enum
{
	ALI_POCKET_METHOD_GET = 0x00,
	ALI_POCKET_METHOD_SET = 0x01,
	ALI_POCKET_METHOD_SERVICE = 0x02,
	ALI_POCKET_METHOD_ACK = 0xFF,
} AliPacketMethod_t;

typedef struct
{
	uint8_t cmd;
	uint8_t method;
	uint32_t seq_num; 
	uint8_t type_id;
	uint8_t attr_id;
	char *p_data;
	uint8_t ack;
} AliPacketStruct_t;

typedef enum
{
	ALI_WIFI_STATE_IDLE = 0,
	ALI_WIFI_STATE_SMART_CONFIG = 1,
	ALI_WIFI_STATE_AP_CONFIG = 2,
	ALI_WIFI_STATE_HOST_CONFIG = 3,
	ALI_WIFI_STATE_ROUTE_CONNECTED = 4,
	ALI_WIFI_STATE_ROUTE_DISCONNECTED = 5,
	ALI_WIFI_STATE_PASSWORK_ERROR = 6,
	ALI_WIFI_STATE_CLOUD_CONNOECTED = 7,
	ALI_WIFI_STATE_CLOUD_DISCONECTED = 8,
	ALI_WIFI_STATE_PHONE_CONNOECTED = 9,
	ALI_WIFI_STATE_LOSE_POWER = 10,
	ALI_WIFI_STATE_POWER_UP = 11,
	ALI_WIFI_STATE_OTHER,
} AliWifiState_t;

typedef struct
{
	uint32_t system;
	uint32_t utc;
	uint16_t year;
	uint8_t mon;
	uint8_t day;
	uint8_t week;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} AliUtcStruct_t;

typedef struct
{
	int8_t timezone;
	int8_t summertime;
} AliTimeZone_t;

typedef enum
{
	ALI_WM_PWOER_OFF      = 0,
	ALI_WM_SLEEP          = 1,
	ALI_WM_USERINTERFACE  = 2,
	ALI_WM_RANDOM         = 3,
	ALI_WM_WALLFOLLOW     = 4,
	ALI_WM_SPOT           = 5,
	ALI_WM_NAV            = 6,
	ALI_WM_VIRTUALWALL    = 7,
	ALI_WM_GOHOME         = 8,
	ALI_WM_CHARGE_STATION = 9,
	ALI_WM_REMOTE         = 10,
	ALI_WM_CHARGE_WIRE    = 11,
	ALI_WM_PAUSE          = 12,
	ALI_WM_TEMP_SPOT      = 13,
	ALI_WM_OTA            = 14,
	ALI_WM_AREA           = 19,
	ALI_WM_CONTINUE       = 21,
	ALI_WM_BACKHOME       = 24,
	ALI_WM_CHARGE_NAV     = 25,
} AliWorkMode_t;

typedef struct 
{
	uint8_t resend_flag;   
	uint8_t report_switch;
	uint8_t wifi_connected_start;	
	uint32_t start_time;   
	uint32_t cleaned_area; 
	uint32_t cleaned_time; 
	Point16_t list[ALI_REALMAP_SIZE+1];
	uint8_t list_cnt;
	uint8_t list_input;
	uint8_t list_output;
	uint8_t list_busy;
	Point16_t busy_list[ALI_REALMAP_BUSY_SIZE+1];
	uint8_t busy_list_cnt;
	uint8_t busy_list_input;
	uint8_t busy_list_output;
	int16_t map_x_max;
	int16_t map_x_min;	
	int16_t map_y_max;
	int16_t map_y_min;
} AliRealMapStruct_t;

typedef struct 
{
	uint8_t switch_state;
	uint8_t start_hour;
	uint8_t start_min;
	uint8_t end_hour;
	uint8_t end_min;
}AliVoiceMute_t;

typedef enum
{
	ALI_CLEAN_DIR_FORWARD = 1,
	ALI_CLEAN_DIR_BACK,
	ALI_CLEAN_DIR_LEFT,
	ALI_CLEAN_DIR_RIGHT,
	ALI_CLEAN_DIR_PAUSE,
}AliCleanDir_t;

typedef struct
{
	CellState_t status;
	uint8_t length;
}AliMapCompress_t;

typedef enum
{
	ALI_CLEAN_TYPE_MAP_DATA = 1,
	ALI_CLEAN_TYPE_PATH_DATA =2,
	ALI_CLEAN_TYPE_VIRTUALWALL_DATA = 3,
	ALI_CLEAN_TYPE_ZONE_DATA = 4,
	ALI_CLEAN_TYPE_CHHARGE_DATA = 5,
	ALI_CLEAN_TYPE_FORBIDAREA_DATA = 6,
	ALI_CLEAN_TYPE_EDITAREA_DATA = 7,
} AliCleanType_t;

typedef enum
{
	ALI_HSR_NONE = 0,
	ALI_HSR_APP,
	ALI_HSR_REMOTE,
	ALI_HSR_BUTTON,
	ALI_HSR_NORMAL_SCHA,
	ALI_HSR_ROOM_SCHA,
	ALI_HSR_AREA_SCHA,
	ALI_HSR_AREA,
	ALI_HSR_ROOM,
}AliHistoryStartReason_t;

typedef enum
{
	ALI_HER_NONE	= 0,
	ALI_HER_CLEAN_FINISH,
	ALI_HER_LOW_POWER,
	ALI_HER_REMOTE,
	ALI_HER_BUTTON,
	ALI_HER_APP,
	ALI_HER_PICK_UP,
	ALI_HER_ERROR,
	ALI_HER_UNFINISHED,
}AliHistoryEndReason_t;

typedef enum
{
	ALI_HC_CALCULATE_POCKET_NUM = 0,
	ALI_HC_START_ADD_HISTORY,
	ALI_HC_CONTINUE_ADD_HISTORY,
}AliHistoryCompression_t;

typedef struct 
{
	uint8_t is_history_remain;
	uint8_t is_last_pocket_finish;
	int16_t stop_dir_idx;
	int16_t stop_scan_idx;
	uint8_t report_switch;	
	uint32_t start_time;
	Point16_t origin_cell;
	AliCleanType_t type;

	AliHistoryStartReason_t start_rsn;
	AliHistoryEndReason_t stop_rsn;
	uint16_t  total_area; //unit dm2
	uint16_t total_time; //unit min
	uint8_t packet_id;   //from 1
	uint8_t packet_num; //
	uint16_t row_width;

	AliMapCompress_t list[ALI_HISTORY_MAP_SIZE+1];
	uint8_t list_cnt;
	uint8_t list_input;
	uint8_t list_output;
	uint8_t list_busy;
} AliHistoryMapStruct_t;

typedef struct
{
	uint8_t cur_mode;
	uint8_t clean_over_value;
	uint8_t clean_set_value;
	uint8_t dry_over_value;
	uint8_t dry_set_value;	
} AliCleanLoopCnt_t;

typedef struct 
{
	uint32_t robot_type;
	uint32_t hardware_version;
	uint32_t software_version;	
}AliRobotInfo_t;

typedef struct 
{
	uint8_t mac[6];
	uint8_t ip[4];
	uint8_t signal_power;
	uint8_t version[4];
	uint8_t is_receive;
}AliWifiSysInfo_t;

typedef struct 
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t weekday;

	uint8_t none;
	uint8_t hour;
	uint8_t min;
	uint8_t second;

	uint32_t gap;
	uint32_t utc;
	uint32_t utc_zero_zone;
}AliTimeCalibration_t;

typedef struct
{
	uint8_t clean_mode;
	uint8_t workmode_switch;
	uint8_t set_workmode;
	uint8_t cur_workmode;	
	int32_t utc_offset;	
	int8_t time_zone;		
	AliUtcStruct_t time;	
	AliWifiState_t set_wifi_state;
	AliWifiState_t cur_wifi_state;
	uint8_t is_working;
	uint8_t is_wifi_connecting;
	uint8_t is_wifi_connected;
	AliRealMapStruct_t real_map;
	uint8_t is_start_nav;
	uint8_t battery_pct;
	AliVoiceMute_t voice_mute;
	AliWifiSysInfo_t wifi_sysinfo;
	uint8_t voice_mute_set_state;
	uint8_t voice_mute_cur_state;	
	uint8_t is_parameter_update;
	
	uint8_t set_pump_spd;
	uint8_t cur_pump_spd;
	uint8_t pump_spd_ack;

	uint8_t set_mainbrush_spd;
	uint8_t cur_mainbrush_spd;
	uint8_t mainbrush_spd_ack;	

	uint8_t set_mov_spd;
	uint8_t cur_mov_spd;
	uint8_t mov_spd_ack;
	
	uint8_t find_robot;
	uint8_t find_robot_cnt;
	uint8_t error_rsn;

	uint8_t over_cleanloop_cnt;
	uint8_t	set_cleanloop_cnt;
	uint8_t cur_set_cleanloop_cnt;

	uint8_t over_dryloop_cnt;
	uint8_t	set_dryloop_cnt;
	uint8_t cur_set_dryloop_cnt;

	AliHistoryMapStruct_t history_map;
	AliRobotInfo_t robot_info;
	uint8_t factory_test_mode;
	uint32_t factorytest_seqnum;
	uint8_t factorytest_ack_ok;
	uint32_t ack_seqnum;
	uint8_t ack_ok;
	uint32_t test_time;
	uint32_t get_mac_time;
	uint32_t function_time;
	uint8_t tx_stop;
	uint8_t station_start_state;
	uint32_t seq_num;

	AliCleanDir_t clean_dir;
	uint8_t update_time_calibration;
	uint8_t update_work_mode;
	uint8_t update_clean_dir;
	uint8_t update_clean_cycle;
	uint8_t update_reset_factory;
	uint8_t update_wifi_state;
	uint8_t update_time_zone;
	uint8_t update_voice_mute;
	uint8_t update_mainbrush;
	uint8_t update_waterflow;
	uint8_t update_mov_speed;
	uint8_t update_unbanding;
	Point16_t robot_cell;
	int16_t robot_angle;

	uint8_t tx_queue_msg_cnt;
	uint8_t tx_queue_msg_input;
	uint8_t tx_queue_msg_output;
	uint16_t tx_size;
	AliTxMsgStruct_t tx_queue_list[ALI_TX_QUEUE_MAX+1];
	char tx_buf[ALI_TX_BUF_MAX];
	char rx_array[2][ALI_RX_BUF_TEMP_SIZE + 1];
	volatile uint8_t rx_buffer[ALI_RX_BUF_PRE_SIZE + 1];
	AliPacketStruct_t  rx_packet;

	AliTimeCalibration_t time_calibration;
} AliStruct_t;

typedef CellState_t (*AliCloud_MapGetCell)(int16_t x, int16_t y);
typedef void (*AliCloud_InsertTheAck)(AliPacketStruct_t *rec_packet);
typedef void(*AliCloud_SetPowerState)(uint8_t state);

#endif





































