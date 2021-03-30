#include "modbus_handlers.h"
#include "BAM.h"




static u8 BAM_values[BAM_CHANNELS_NUM] = {0};

u8 BAM_value_access(u8 mode, u16 adr, u16 * val)
{
  u16 i = adr - HOLDING_REGISTER_ADR_OFFSET;
  if(mode == REG_READ)
  {
    *val = BAM_values[i];
    return NO_ERROR;
  }  
  else
  {
    if(*val > BAM_MAX_VALUE) return ILLEGAL_DATA_VALUE;
    BAM_values[i] = *val;
    return NO_ERROR;
  }
}

u8 BAM_action_access(u8 mode, u16 adr, u16 * val)
{
  if(mode == REG_READ)
  {
    *val = 0;
    return NO_ERROR;
  }
  else
  {
    switch (*val)
    {
    case BAM_ACT_UPDATE: 
      {
        bam_update();
        break;
      }
    case BAM_ACT_INIT: 
      {
        break;
      }
    default: 
      {
        return ILLEGAL_DATA_VALUE;
      }
    }
    return NO_ERROR;
  }
}

u8 animation_access     (u8 mode, u16 adr, u16 * val)
{
  return NO_ERROR;
}

u8 modbus_adr_access    (u8 mode, u16 adr, u16 * val)
{
  return NO_ERROR;
}

u8 baudrate_access      (u8 mode, u16 adr, u16 * val)
{
  return NO_ERROR;
}

u8 action_access        (u8 mode, u16 adr, u16 * val)
{
  return NO_ERROR;
}

