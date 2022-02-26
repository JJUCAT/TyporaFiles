#include "Include.h"

void Flash_Unlock(void)
{
  if((FLASH->CR & FLASH_CR_LOCK) != RESET)
  {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }  
}

void Flash_Lock(void)
{
  FLASH->CR |= FLASH_CR_LOCK;
}

uint16_t Flash_GetSector(uint32_t adr)
{
    uint16_t sector = 0;

    if((adr < FLASH_ADDR_SECTOR_1) && (adr >= FLASH_ADDR_SECTOR_0))
    {
        sector = FLASH_Sector_0;  
    }
    else if((adr < FLASH_ADDR_SECTOR_2) && (adr >= FLASH_ADDR_SECTOR_1))
    {
        sector = FLASH_Sector_1;  
    }
    else if((adr < FLASH_ADDR_SECTOR_3) && (adr >= FLASH_ADDR_SECTOR_2))
    {
        sector = FLASH_Sector_2;  
    }
    else if((adr < FLASH_ADDR_SECTOR_4) && (adr >= FLASH_ADDR_SECTOR_3))
    {
        sector = FLASH_Sector_3;  
    }
    else if((adr < FLASH_ADDR_SECTOR_5) && (adr >= FLASH_ADDR_SECTOR_4))
    {
        sector = FLASH_Sector_4;  
    }
    else if((adr < FLASH_ADDR_SECTOR_6) && (adr >= FLASH_ADDR_SECTOR_5))
    {
        sector = FLASH_Sector_5;  
    }
    else if((adr < FLASH_ADDR_SECTOR_7) && (adr >= FLASH_ADDR_SECTOR_6))
    {
        sector = FLASH_Sector_6;  
    }
    else if((adr < FLASH_ADDR_SECTOR_8) && (adr >= FLASH_ADDR_SECTOR_7))
    {
        sector = FLASH_Sector_7;  
    }
    else if((adr < FLASH_ADDR_SECTOR_9) && (adr >= FLASH_ADDR_SECTOR_8))
    {
        sector = FLASH_Sector_8;  
    }
    else if((adr < FLASH_ADDR_SECTOR_10) && (adr >= FLASH_ADDR_SECTOR_9))
    {
        sector = FLASH_Sector_9;  
    }
    else if((adr < FLASH_ADDR_SECTOR_11) && (adr >= FLASH_ADDR_SECTOR_10))
    {
        sector = FLASH_Sector_10;  
    }
    else
    {
        sector = FLASH_Sector_11;  
    }
    return sector;
}

FLASH_Status Flash_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_WRPERR) != (uint32_t)0x00)
    { 
      flashstatus = FLASH_ERROR_WRP;
    }
    else
    {
      if((FLASH->SR & FLASH_FLAG_RDERR) != (uint32_t)0x00)
      { 
        flashstatus = FLASH_ERROR_RD;
      } 
      else 
      {
        if((FLASH->SR & (uint32_t)0xE0) != (uint32_t)0x00)
        {
          flashstatus = FLASH_ERROR_PROGRAM; 
        }
        else
        {
          if((FLASH->SR & FLASH_FLAG_OPERR) != (uint32_t)0x00)
          {
            flashstatus = FLASH_ERROR_OPERATION;
          }
          else
          {
            flashstatus = FLASH_COMPLETE;
          }
        }
      }
    }
  }
  return flashstatus;
}

FLASH_Status Flash_WaitForLastOperation(void)
{ 
  __IO FLASH_Status status = FLASH_COMPLETE;
   
  status = Flash_GetStatus();

  while(status == FLASH_BUSY)
  {
    status = Flash_GetStatus();
  }
  return status;
}


FLASH_Status Flash_EraseSector(uint32_t sector, uint32_t size)//FLASH_PSIZE_WORD
{
  FLASH_Status status = FLASH_COMPLETE;
  status = Flash_WaitForLastOperation();
  if(status == FLASH_COMPLETE)
  { 
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= size;
    FLASH->CR &= FLASH_SECTOR_MASK;
    FLASH->CR |= FLASH_CR_SER | sector;
    FLASH->CR |= FLASH_CR_STRT;
    
    status = Flash_WaitForLastOperation();
    
    FLASH->CR &= (~FLASH_CR_SER);
    FLASH->CR &= FLASH_SECTOR_MASK; 
  }
  return status;
}

uint8_t Flash_Erase(uint32_t adr_start, uint32_t adr_end)
{
    uint32_t start_sector, end_sector;
    uint8_t i;

    start_sector = Flash_GetSector(adr_start); 
    end_sector = Flash_GetSector(adr_end);

    FLASH->ACR &= ~ (1<<10); 
    for (i = start_sector; i <= end_sector; i += 8)
    {
        if (Flash_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
        { 
            return 1;   
        }
    }
    FLASH->ACR |= (1<<10); 
    return 0;   
}

FLASH_Status Flash_ProgramByte(uint32_t adr, uint8_t data)
{
  FLASH_Status status = FLASH_COMPLETE;

  status = Flash_WaitForLastOperation();
  
  if(status == FLASH_COMPLETE)
  {
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_BYTE;
    FLASH->CR |= FLASH_CR_PG;
  
    *(__IO uint8_t*)adr = data;
        
    status = Flash_WaitForLastOperation();

    FLASH->CR &= (~FLASH_CR_PG);
  } 
  return status;
}

void Flash_WriteByte( uint32_t adr, uint8_t* data ,uint32_t length)
{
  uint32_t i = 0;
  for (i = 0; i < length; i++)
  {
    if (Flash_ProgramByte(adr, *(data+i)) == FLASH_COMPLETE)
    {
        adr += 1;
    }
  }
}

void Flash_WriteOneByte(uint32_t adr,uint8_t data)
{
  Flash_Unlock();
  if(Flash_ProgramByte(adr,data) == FLASH_COMPLETE)
  {
    Usprintf("flash write data:%d\n",data);	
  }
  Flash_Lock();
}

uint8_t Flash_ReadByte( uint32_t adr)
{
    uint8_t data;

    data = *(__IO uint8_t *)adr;

    return data;	    
}

void Flash_Read( uint32_t adr, uint8_t* data ,uint32_t length)
{
    uint32_t i = 0;

    for (i = 0; i < length; i++)
    {	
        *(data+i) = *(__IO uint8_t *)adr;
        adr += 1;
    }	    
}










