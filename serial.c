#include "serial.h"
#include "stm8s_uart1.h"

static void UART1_RX_Cmd(FunctionalState state)
{
  if(state == ENABLE)
  {
    UART1->CR2 |= (uint8_t)UART1_CR2_REN;  
  }
  else
  {
    UART1->CR2 &= (uint8_t)(~UART1_CR2_REN);  
  }
}

u8 serial_get_rx_num(serial_t* desc)
{
  return ringbuffer_get_num(&desc->rx_ringbuf);
}


void serial_open(serial_t* desc, 
                 uint32_t BaudRate,
                 u8   halfduplex,
                 u8 * buf, 
                 u8   buf_len)
{
  desc->halfduplex_flag = halfduplex;
  UART1_Init (BaudRate, 
              UART1_WORDLENGTH_8D,
              UART1_STOPBITS_1,
              UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, 
              UART1_MODE_TXRX_ENABLE);
  
  ringbuffer_init(&desc->rx_ringbuf, buf, buf_len);
  UART1_ITConfig(UART1_IT_RXNE, ENABLE);
  UART1_HalfDuplexCmd(ENABLE);
  UART1_Cmd(ENABLE);
}

//Non-blocking reads all the data available in the buffer at the moment.
//returns the amount of data read.
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

//returns 0 if data was succesfully read
//else returns 1
u8 serial_read_byte(serial_t* desc, u8 * c)
{ 
  return ringbuffer_get(&desc->rx_ringbuf, c);
}

//Teturns 1 if transmission was started
//else returns 0.
//The transmission may not start due to another transmission in progress
//If parameter len is zero, buffer is considered to be 0 terminated string and handled accordingly
u8 serial_write(serial_t* desc, u8 * buf, u16 len)
{
  if (desc->status_reg & TX_IN_PROGRESS)  { return 0;}

  if(len == 0)
  {
    while(*(buf+len)){len++;}
  }

  desc->tx_buf = buf;
  desc->tx_cnt = len;
  
  if(desc->halfduplex_flag)    
  {
    UART1_RX_Cmd(DISABLE);
  }
  
  UART1_ITConfig(UART1_IT_TXE, ENABLE);
  return 1;
}

//well, this is a good example of ugly code that is caused by imperfections of STM8 SPL 
//that I just don't want to deal with at the moment (or ever in my life)
void uart1_rx_isr(serial_t * desc)
{
  u8 status = UART1->SR;
  u8 data = UART1->DR;
  ringbuffer_put(&desc->rx_ringbuf, data);
}

void uart1_tx_isr(serial_t * desc)
{
  if(desc->tx_cnt)
  {
    UART1->DR = *(desc->tx_buf++);
    desc->status_reg |= TX_IN_PROGRESS;
    desc->tx_cnt--;
  }
  else
  {
    while(UART1_GetFlagStatus(UART1_FLAG_TC)==RESET){} //wait for the final byte to be actually transmitted
    UART1_ITConfig(UART1_IT_TXE, DISABLE);
    desc->status_reg &= ~TX_IN_PROGRESS;
    if(desc->halfduplex_flag)    
    {
      UART1_RX_Cmd(ENABLE);
    }
  }
}