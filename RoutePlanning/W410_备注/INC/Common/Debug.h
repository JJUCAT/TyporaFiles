#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "config.h"
#include "Include.h"



#define  DEBUG_SIZE            60
#define  DEBUG_LENGTH          100


extern int usprintf(char *out, const char *format, ...);

#ifdef DEBUG_ENABLE
	#define Usprintf(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					Debug_PrintString(outString); \
				  }
	#define printf(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					ups3(outString); \
				}

#else
	#define Usprintf(fmt, ...)	{do {;} while (0);}
	#define printf(fmt, ...)	{do {;} while (0);}
				
#endif
					
#ifdef DEBUG_ENABLE_ACTION
	#define Usprintf_Action(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					Debug_PrintString(outString); \
				}
	#define printfaction(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					ups3(outString); \
				}				
#else
	#define Usprintf_Action(fmt, ...)	{do {;} while (0);}
#endif	
	
					
#ifdef DEBUG_WIFI 
	#define Usprintf_Wifi(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					Debug_PrintString(outString); \
				}
	#define printfwifi(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					ups3(outString); \
				}
#else

	#define Usprintf_Wifi(fmt, ...)	{do {;} while (0);}
#endif

#ifdef DEBUG_PATHPLANNING 
	#define Usprintf_PathPlanning(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					Debug_PrintString(outString); \
				}
	#define printfpathplanning(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					ups3(outString); \
				}
#else

	#define Usprintf_PathPlanning(fmt, ...)	{do {;} while (0);}
#endif

#ifdef DEBUG_TEST 
	#define Usprintf_Test(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					Debug_PrintString(outString); \
				}
	#define printftest(...)	{						\
					usprintf(outString, ##__VA_ARGS__);	\
					ups3(outString); \
				}
#else

	#define Usprintf_Test(fmt, ...)	{do {;} while (0);}
#endif

#ifdef DEBUG_ENABLE
extern char g_debug_fifo[DEBUG_SIZE][DEBUG_LENGTH];
extern char outString[256];
#endif

void Debug_PrintString(char *p_data);
void Debug_PrintStringByLength(char *p_data,uint8_t length);
void Debug_Cycle(void);

#endif


/***********************************************************/

