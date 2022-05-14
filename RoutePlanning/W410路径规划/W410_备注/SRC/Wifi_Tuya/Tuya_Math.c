#include "Include.h"


const DataPoint_t g_download_cmd[] =
{
  {DPID_POWER, DP_TYPE_BOOL},
  {DPID_SWITCH_GO, DP_TYPE_BOOL},
  {DPID_MODE, DP_TYPE_ENUM},
  {DPID_ELECTRICITY_LEFT, DP_TYPE_VALUE},
  {DPID_CLEANAREA, DP_TYPE_VALUE},
  {DPID_STATUS, DP_TYPE_ENUM},
  {DPID_FAULT, DP_TYPE_FAULT},
  {DPID_WATER_CONTROL, DP_TYPE_ENUM},
  {DPID_DIRECTIONCONTROL, DP_TYPE_ENUM},
  {DPID_CLEAN_RECORD, DP_TYPE_STRING},
  {DPID_EDGEBRUSH, DP_TYPE_VALUE},
  {DPID_ROLLBRUSH, DP_TYPE_VALUE},
  {DPID_FILTER, DP_TYPE_VALUE},
  {DPID_RESETFILTER, DP_TYPE_BOOL},
  {DPID_RESETROLLBRUSH, DP_TYPE_BOOL},
  {DPID_RESETEDGEBRUSH, DP_TYPE_BOOL},
  {DPID_RESUME, DP_TYPE_BOOL},
  {DPID_MAX, DP_TYPE_ENUM},
  {DPID_SEEK, DP_TYPE_BOOL},
  {DPID_CLEARTIME, DP_TYPE_VALUE},
  {DPID_WATERLEVELCONTROL,DP_TYPE_ENUM},
  {DPID_TONESWITCH,DP_TYPE_BOOL},
};

unsigned char TuyaMath_GetCheckSum(unsigned char *pack, unsigned short pack_len)
{
    unsigned short i;
    unsigned char check_sum = 0;

    for(i = 0; i < pack_len; i ++)
    {
        check_sum += *pack ++;
    }

    return check_sum;
}


void *TuyaMath_Memcpy(void *dest, const void *src, unsigned short count)  
{  
    unsigned char *pdest = (unsigned char *)dest;  
    const unsigned char *psrc  = (const unsigned char *)src;  
    unsigned short i;

    if(dest == NULL || src == NULL)
    { 
     return NULL;
    }

    if((pdest <= psrc) || (pdest > psrc + count))
    {  
        for(i = 0; i < count; i ++)
        {  
            pdest[i] = psrc[i];  
        }  
    }
    else
    {
        for(i = count; i > 0; i --)
        {  
            pdest[i - 1] = psrc[i - 1];  
        }  
    }  

    return dest;  
}

unsigned long TuyaMath_Strlen(unsigned char *str)  
{
    unsigned long len = 0;
    if(str == NULL)
    { 
        return 0;
    }

    for(len = 0; *str ++ != '\0'; )
    {
        len ++;
    }

    return len;
}

unsigned char TuyaMath_GetDownloadCmdTotal(void)
{
    return(sizeof(g_download_cmd) / sizeof(g_download_cmd[0]));
}

unsigned char TuyaMath_GetDowmloadDpidIndex(unsigned char dpid)
{
  unsigned char index;
  unsigned char total = TuyaMath_GetDownloadCmdTotal();
  
  for(index = 0; index < total; index ++)
  {
    if(g_download_cmd[index].dp_id == dpid)
    {
      break;
    }
  }
  
  return index;
}

unsigned char TuyaMath_GetDpType(unsigned char index)
{
    return g_download_cmd[index].dp_type;
}

unsigned char TuyaMath_GetDpDownloadEnum(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

unsigned char TuyaMath_GetDpDownloadBool(const unsigned char value[],unsigned short len)
{
  return(value[0]);
}








