#include "serial.h"
#include "stm8s_uart1.h"


void serial_open(serial_t* desc, 
                 uint32_t BaudRate, 
                 u8 * buf, 
                 u8   buf_len)
{
  UART1_Init (115200, 
              UART1_WORDLENGTH_8D,
              UART1_STOPBITS_1,
              UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, 
              UART1_MODE_TXRX_ENABLE);
  
  ringbuffer_init(&desc->rx_ringbuf, buf, buf_len);
  UART1_ITConfig(UART1_IT_RXNE, ENABLE);
  UART1_ITConfig(UART1_IT_IDLE, ENABLE);
  UART1_HalfDuplexCmd(ENABLE);
  UART1_Cmd(ENABLE);
}

//returns the amount of data read
u8 serial_read(serial_t* desc, u8 * buf, u16 len)
{ 
  u16 n = len;
  while(len)
  {
  if(ringbuffer_get(&desc->rx_ringbuf, buf++)) break;
  len--;
  }
  return n - len;
}

//returns the amount of data placed to outgoing buffer. If buffer is full returns 0
void serial_write(serial_t* desc, u8 * buf, u16 len)
{
  if(len == 0) return; //not sure if this check actually is needed
  while (desc->status_reg & TX_IN_PROGRESS){}
  desc->tx_buf = buf;
  desc->tx_cnt = len;
  UART1_ITConfig(UART1_IT_TXE, ENABLE);
  UART1_ITConfig(UART1_IT_TC, ENABLE);
}

//well, this is a good example of ugly code that is caused by imperfections of STM8 SPL 
//that I just don't want to deal with at the moment (or ever in my life)
void uart1_rx_isr(serial_t * desc)
{
    u8 status = UART1->SR;
    if(status & UART1_IT_RXNE)
    {
        u8 data = UART1->DR;
        ringbuffer_put(&desc->rx_ringbuf, data);
        desc->status_reg |= RX_IN_PROGRESS;
    }
    if(status & UART1_IT_IDLE)
    {
        desc->status_reg &= ~RX_IN_PROGRESS;
    }
}



void uart1_tx_isr(serial_t * desc)
{
  u8 status = UART1->SR;
  if(status & UART1_IT_TXE)
  {
      if(desc->tx_cnt)
      {
        UART1->DR = desc->tx_buf[desc->tx_cnt--];
        desc->status_reg |= TX_IN_PROGRESS;
      }
      else
      {
        UART1_ITConfig(UART1_IT_TXE, DISABLE);
      }
  }
 
  if(status & UART1_IT_TC)
  {
    UART1_ITConfig(UART1_IT_TC, DISABLE);
    desc->status_reg &= ~TX_IN_PROGRESS;
  }
}