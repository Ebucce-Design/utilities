#ifndef __ENCODER
#define __ENCODER


#include "stm8s_gpio.h"


typedef enum 
{
  ENC_NO_ACTION = 0, 
  ENC_TURN_DIR1 = 1,
  ENC_TURN_DIR2 = 2
}EncoderAction;

typedef struct
{
  GPIO_TypeDef* port_a;
  GPIO_Pin_TypeDef pin_a;
  GPIO_TypeDef* port_b;
  GPIO_Pin_TypeDef pin_b;
  u8 new_status;
  u8 status_b;
  u8 status_a;
  EncoderAction action;
}encoder_t;

void encoder_init(encoder_t * encoder);
EncoderAction encoder_read(encoder_t * encoder);
#endif
