
#include "modbus.h"
#include "modbus_handlers.h"

static u8 error     = NO_ERROR; //this global variable should contain error code in case of any during data process
static u8 address   = 0;              
static u8 bufsize   = 0;        //this is a maximum buffer size for holding the response before sending it. 

// Compute the MODBUS RTU CRC

static u8 access_holding_reg (u16 adr, u8 mode, u16 * val)
{
  for(u8 i = 0; i < ARRAY_SIZE(register_map); i++)
  {
    if(adr == register_map[i].adr)
    {
    return register_map[i].reg_access_func(mode, adr, val);
    }
  }
  return ILLEGAL_DATA_ADDRESS;
}

#ifdef WRITE_SINGLE_REGISTER
static u8 write_single_register_handler(u8 * data)
{
  u16 adr = ((*data++) << 8);
      adr += *(data++);  
  u16 n   = ((*data++) << 8); 
      n  += *(data++); // number of registers 
  
  error = access_holding_reg(adr, REG_WRITE, &n);
  if (error) {return 0;}
  return 4; 
}
#endif

#ifdef READ_HOLDING_REGISTERS
static u8 read_holding_registers_handler(u8 * data)
{ 
  u16 adr =  *(data) << 8;
      adr += *(data + 1);  
  u16 n   = *(data + 2) << 8; 
      n  += *(data + 3); // number of registers
      
  *data++ = n * 2; // BYTE COUNT, NOT REGISTER COUNT
  
  u16 val = 0;  
  if(bufsize - 4 < (n * 2) + 1)
  {
    error = SERVER_DEVICE_FAILURE;
    return 0;
  }
  u16 c = n;
  while(c--)
  {
    error = access_holding_reg(adr++, REG_READ, &val);
    if(error) {return 0;}  
    *data++ = (val & 0xFF00) >>8;
    *data++ = val & 0xFF; 
  }
 return  (n * 2) + 1;
}
#endif

#ifdef WRITE_MULTIPLE_REGISTERS
static u8 write_multiple_registers_handler (u8 * data)
{ 
  u16 adr = ((*data++) << 8);
      adr += *(data++);  
  u16 n   = ((*data++) << 8); 
      n  += *(data++); // number of registers 
 
  if(n * 2 != *(data++)) 
  {
    error = ILLEGAL_DATA_VALUE;
    return 0;
  }
  
  u16 val = 0;

  while(n--)
  {
  val =  *(data++) << 8;
  val += *(data++);
  error = access_holding_reg(adr++, REG_WRITE, &val);
  if(error) { return 0; }
  }
 return 4;
}
#endif

static const modbus_code_map_t supported_func_codes[] = 
{
#ifdef READ_HOLDING_REGISTERS
  {READ_HOLDING_REGISTERS,      read_holding_registers_handler},
#endif
#ifdef WRITE_SINGLE_REGISTER
  {WRITE_SINGLE_REGISTER,       write_single_register_handler},
#endif
#ifdef WRITE_MULTIPLE_REGISTERS
  {WRITE_MULTIPLE_REGISTERS,    write_multiple_registers_handler},
#endif 
};

static u16 crc16(u8* buf, u16 len)
{
  u16 crc = 0xFFFF;
  
  for (u16 pos = 0; pos < len; pos++) {
    crc ^= (u16)buf[pos];        // XOR byte into least sig. byte of crc
  
    for (int i = 8; i != 0; i--) // Loop over each bit
    {    
      if ((crc & 0x0001) != 0)   // If the LSB is set
      {      
        crc >>= 1;               // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                       // Else LSB is not set
        crc >>= 1;               // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  u8 a = crc & 0xFF;
  crc >>= 8;
  crc |= a << 8;
  return crc;  
}

void modbus_init(u8 adr, u8 size)
 {
  if(adr < 247) 
  {
    address = adr;
  }
  bufsize = size;
 }
 

//returns length of response data part if everything is OK 
//otherwise returns 0
u16 modbus_process_frame(u8 * data, u16 datalen)
{   
  u8 adr  = data[0];
  u8 code = data[1];
  u8 len  = 2;

  if(adr != address) return 0;
  
  u16 crc = (data[datalen - 2]<<8) + data[datalen - 1];
  if(crc != crc16(data, datalen - 2)) 
  {
    return 0; //CRC ERROR
  }
  
  error = ILLEGAL_FUNCTION;
  
  for(u8 i = 0; i < ARRAY_SIZE(supported_func_codes); i++)
  {
    if(code == supported_func_codes[i].code) 
    {
      len += supported_func_codes[i].handler(&data[2]);
      break;
    }
  }

  if(adr == 0) {return 0;} // device should not answer to broadcast messsages with address 0

  if(error)
  {
    data[1] |= 0x80;
    data[2]  = error;
    len = 3;
  }
    
  crc = crc16(data, len);
  
  data[len++] = (crc & 0xFF00) >> 8;
  data[len++] =  crc & 0x00FF;
  
  return len;
}