
#include "encoder.h"

static inline u8 get_pin_state(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin)
{
  if(GPIOx->IDR & (uint8_t)GPIO_Pin) return 1;
  return 0;
}

void encoder_init(encoder_t * encoder)
{
  GPIO_Init(encoder->port_a, encoder->pin_a, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(encoder->port_b, encoder->pin_b, GPIO_MODE_IN_FL_NO_IT);
  encoder->new_status = get_pin_state(encoder->port_a, encoder->pin_a);
}

EncoderAction encoder_read(encoder_t * encoder)
{
    encoder->new_status = get_pin_state(encoder->port_a, encoder->pin_a);
    if(encoder->status_a != encoder->new_status)
    {
     encoder->status_a = encoder->new_status;
     if(encoder->new_status == get_pin_state(encoder->port_b, encoder->pin_b))
     {
      return ENC_TURN_DIR1;
     } 
     else
     {
      return ENC_TURN_DIR2;
     }
    }     
return ENC_NO_ACTION;
} 
