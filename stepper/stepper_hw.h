#ifndef STEPPER_HW_H
#define STEPPER_HW_H

#include "stm8s_gpio.h"
#include "stm8s.h"

typedef enum {DIR_CW = 0, DIR_CCW = !DIR_CW} stepper_direction_t;


typedef struct
{
  GPIO_TypeDef * port;
  GPIO_Pin_TypeDef pin;
}
pin_t;


typedef struct
{
  pin_t coil1_A;
  pin_t coil1_B;
  pin_t coil2_A;
  pin_t coil2_B;
  s8 phase;
  u8 phase_step; //this is 2 for full step and 1 for half step
  stepper_direction_t dir; 
  void (*timer_update)(u16);
  void (*timer_start)(void);
  void (*timer_stop)(void);
}stepper_hw_desc_t;

void stepper_step(stepper_hw_desc_t * d);
void stepper_off(stepper_hw_desc_t * d);


#endif