#ifndef __MODBUS
#define __MODBUS

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#include "types.h"

#define DISCRETE_INPUTS_ADR_OFFSET  10001
#define COILS_ADR_OFFSET            20001
#define INPUTS_ADR_OFFSET           30001
#define HOLDING_REGISTER_ADR_OFFSET 40001

//modbus error codes
#define NO_ERROR              00
#define ILLEGAL_FUNCTION      01 
#define ILLEGAL_DATA_ADDRESS  02 
#define ILLEGAL_DATA_VALUE    03 
#define SERVER_DEVICE_FAILURE 04
#define ACKNOWLEDGE           05 
#define SERVER_DEVICE_BUSY    06 
#define MEMORY_PARITY_ERROR   08 

//#define READ_COILS                1
//#define READ_DISCRETE_INPUTS      2
  #define READ_HOLDING_REGISTERS    3
//#define READ_INPUT_REGISTERS      4
//#define WRITE_SINGLE_COIL         5
//#define WRITE_MULTIPLE_COILS      15
  #define WRITE_SINGLE_REGISTER     6
  #define WRITE_MULTIPLE_REGISTERS  16

#define REG_READ        0
#define REG_WRITE       1

typedef struct
{
  u8  code;
  u8 (*handler)(u8 * data);
}modbus_code_map_t;

typedef struct
{
  u16 adr;                                            //register address
  u8 (*reg_access_func)(u8 mode, u16 adr, u16 * val); //callback function which is called to access registrer value.
}reg_map_t;

u16   modbus_process_frame    (u8 * data, u16 datalen);
void  modbus_init             (u8 adr, u8 size);
#endif