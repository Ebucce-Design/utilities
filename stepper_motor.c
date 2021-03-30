#include "stepper_motor.h"
#include "stm8s_tim1.h"
#include "stm8s_tim2.h"

static inline void PIN_WriteLow(pin_t * pin)
{
  pin->port->ODR &= (uint8_t)(~pin->pin);
}

static inline void PIN_WriteHigh(pin_t * pin)
{
  pin->port->ODR |= (uint8_t)(pin->pin);
}

static inline void stepper_timer_update(stepper_motor_t * m, u16 period)  // this should be defined by user
{
  if(m->timer == STEPPER_TIMER1)
  {
  
  }
  if(m->timer == STEPPER_TIMER2)
  {
  TIM2->ARRH = (uint8_t)(period >> 8); 
  TIM2->ARRL = (uint8_t)(period);
  }
  };

void stepper_timer_cmd(stepper_motor_t * m,FunctionalState NewState)
{
  if(m->timer == STEPPER_TIMER2)
  {
  TIM1_Cmd(NewState);
  }
  
  if(m->timer == STEPPER_TIMER2)
  {
  TIM2_Cmd(NewState);
  } 
}

void stepper_init(stepper_motor_t * m, pin_t * A1, pin_t * A2, pin_t * B1, pin_t * B2, u8 timer)
{
  GPIO_Init(A1->port, A1->pin, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(A2->port, A2->pin, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(B1->port, B1->pin, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(B2->port, B2->pin, GPIO_MODE_OUT_PP_LOW_SLOW);
  
  m->A1 = A1;
  m->A2 = A2;
  m->B1 = B1;
  m->B2 = B2;
  
  m->step_phase = 0;
  m->step_state = IDLE;
  m->dir = MOTOR_DIR_CW;
  m->timer_period = 0;
  m->accel_ramp = 0;
  m->steps_to_break = 0;
  m->steps_to_do= 0;
}

void stepper_step(stepper_motor_t * m)
{
  switch(m->step_phase)
  {
  case 0: {
          PIN_WriteLow(m->A1);
          PIN_WriteHigh(m->A2);
          PIN_WriteHigh(m->B1);
          PIN_WriteLow(m->B2);
          m->step_phase++;
          break;
          }
  case 1: {
          PIN_WriteLow(m->A1);
          PIN_WriteHigh(m->A2);
          PIN_WriteLow(m->B1);
          PIN_WriteHigh(m->B2);
          m->step_phase++;
          break;
          }
  case 2: {
          PIN_WriteHigh(m->A1);
          PIN_WriteLow(m->A2);
          PIN_WriteLow(m->B1);
          PIN_WriteHigh(m->B2);
          m->step_phase++;
          break;
          }
  case 3: {
          PIN_WriteHigh(m->A1);
          PIN_WriteLow(m->A2);
          PIN_WriteHigh(m->B1);
          PIN_WriteLow(m->B2);
          m->step_phase = 0;
          break;
          }
  }
  m->steps_to_do--;
  if(m->steps_to_do== 0)
  {
    stepper_timer_cmd(m, DISABLE);
  }
}

void stepper_set_dir(stepper_motor_t * m, u8 dir)
{
  if(m->dir == dir) return;
    m->dir = dir;
    pin_t* tmp = m->A1;
    m->A1 = m->A2;
    m->A2 = tmp;
}

void stepper_move(stepper_motor_t * m, u16 steps)
{


}