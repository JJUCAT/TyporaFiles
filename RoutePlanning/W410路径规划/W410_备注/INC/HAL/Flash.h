#ifndef __FLASH_H
#define __FLASH_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


#define FLASH_ADDR_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define FLASH_ADDR_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define FLASH_ADDR_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define FLASH_ADDR_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define FLASH_ADDR_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define FLASH_ADDR_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define FLASH_ADDR_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define FLASH_ADDR_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define FLASH_ADDR_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define FLASH_ADDR_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define FLASH_ADDR_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define FLASH_ADDR_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */

#define FLASH_SECTOR_MASK       ((uint32_t)0xFFFFFF07)

#define FLASH_START_ADDRESS	    ((uint32_t)0x08004001) 

void Flash_Unlock(void);
void Flash_Lock(void);
uint16_t Flash_GetSector(uint32_t adr);
FLASH_Status Flash_GetStatus(void);
FLASH_Status Flash_WaitForLastOperation(void);
FLASH_Status Flash_EraseSector(uint32_t sector, uint32_t size);
uint8_t Flash_Erase(uint32_t adr_start, uint32_t adr_end);
FLASH_Status Flash_ProgramByte(uint32_t adr, uint8_t data);
void Flash_WriteByte( uint32_t adr, uint8_t* data ,uint32_t length);
uint8_t Flash_ReadByte( uint32_t adr);
void Flash_Read( uint32_t adr, uint8_t* data ,uint32_t length);
void Flash_WriteOneByte(uint32_t adr,uint8_t data);
#endif





