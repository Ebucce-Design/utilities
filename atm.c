#include "atm.h"
#include "serial.h"
u8 crc8(u8 * mem, u8 len) 
{
    return 0;
}

atm_frame_t frame = 0;

void atm_process_frame(atm_frame_t * frame)
{
    if(frame->counter != 0)
    {
        frame->counter--;
        return;
    } 
        atm_process_data(frame.data);    
        atm_send_frame(&f    rame);
    }
}

serial_t serial;
u8 serial_read          (serial_t* desc, u8 * buf, u16 len);

void atm_read_frame(atm_frame_t * frame)
{
    serial_read(&serial, &frame->preamble,1);
    
    if(&frame->preamble != 0x69) 
    {
        //Invalid preamble. WHAT TO DO
    }
    serial_read(&serial, &frame->counter, 2);
    serial_read(&serial, &frame->data_len, 1);
    serial_read(&serial, &frame->data_crc, 1);
    serial_read(&serial, &frame->data, frame->data_len);
}