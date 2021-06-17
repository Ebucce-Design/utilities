#ifndef __ATM_H
#define __ATM_H

#include "types.h"

#define ATM_PREAMBLE        0x69
#define ATM_MAX_COUNTER_VAL 1024
#define ATM_MAX_DATA_LEN    (0xFF - 5)

typedef struct 
{
    u8 preamble; //this should always be 0x69
    u16 counter; //this value is used by device to know if the message is designated to it or not
    u8 data_len; //length of data
    u8 data_crc; //crc of data section only.
    u8 data[ATM_MAX_DATA_LEN]; 
}atm_frame_t;




#endif