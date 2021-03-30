#ifndef __SERIAL
#define __SERIAL

#include "stm8s.h"
#include "ringbuffer.h"
#include "types.h"


#define RX_IN_PROGRESS  BIT0
#define TX_IN_PROGRESS  BIT1

typedef struct
{
    ringbuffer_t rx_ringbuf;
    u8* tx_buf;
    volatile u16 tx_cnt;
    volatile u8 status_reg;
}serial_t;


u8 serial_read          (serial_t* desc, u8 * buf, u16 len);
void serial_write       (serial_t* desc, u8 * buf, u16 len);
void serial_flush_rx    (serial_t * desc);
void serial_open        (serial_t* desc, 
                         uint32_t BaudRate, 
                         u8 * buf, 
                         u8 buf_len);

void uart1_rx_isr(serial_t * desc);
void uart1_tx_isr(serial_t * desc);

static inline u8 serial_is_Receiving(serial_t* desc)
{
  return desc->status_reg & RX_IN_PROGRESS;
}

static inline u8 serial_is_Transmitting(serial_t* desc)
{
  return desc->status_reg & TX_IN_PROGRESS;
}

static inline u8 serial_get_rx_cnt(serial_t * desc)
{
  return ringbuffer_get_num(&desc->rx_ringbuf);
}

static inline void serial_flush_rx(serial_t * desc)
{
  ringbuffer_flush(&desc->rx_ringbuf);
}

#endif