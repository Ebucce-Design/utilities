#ifndef __MODBUS_HANDLERS
#define __MODBUS_HANDLERS

#include "types.h"
#include "modbus.h"

#define BAM_ACT_UPDATE      0x01
#define BAM_ACT_INIT        0x02

u8 BAM_value_access     (u8 mode, u16 adr, u16 * val);
u8 BAM_action_access    (u8 mode, u16 adr, u16 * val);
u8 animation_access     (u8 mode, u16 adr, u16 * val);

u8 modbus_adr_access    (u8 mode, u16 adr, u16 * val);
u8 baudrate_access      (u8 mode, u16 adr, u16 * val);
u8 action_access        (u8 mode, u16 adr, u16 * val);


static const reg_map_t register_map[] = 
{
  //actual BAM values
  {0, BAM_value_access},
  {1, BAM_value_access},
  {2, BAM_value_access},
  {3, BAM_value_access},
  {4, BAM_value_access},
  {5, BAM_value_access},
  {6, BAM_value_access},
  {7, BAM_value_access},
  {8, BAM_value_access},
  {9, BAM_value_access},
  {10, BAM_value_access},
  {11, BAM_value_access},
  {12, BAM_action_access},
  
  {20, modbus_adr_access},
  {21, baudrate_access},
  {22, action_access},
  {23, animation_access}
//Settings etc
};




#endif