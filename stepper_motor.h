#ifndef __STEPPER_MOTOR
#define __STEPPER_MOTOR

#include "stm8s_gpio.h"

#define MOTOR_DIR_CW     0
#define MOTOR_DIR_CCW    1

#define STEPPER_TIMER1  1
#define STEPPER_TIMER2  2

typedef struct
{
  GPIO_TypeDef * port;
  GPIO_Pin_TypeDef pin;
}
pin_t;

typedef enum {IDLE, ACCELERATING, CRUISING, DECELERATING} StepperState_t;

typedef struct
{
  pin_t* A1; //pin 1 of winding A
  pin_t* A2; //pin 2 of winding A
  pin_t* B1; //pin 1 of winding A
  pin_t* B2; //pin 2 of winding A
  u8 timer;
  
  StepperState_t step_state;
  u8 step_phase;
  u8 dir;
  u16 timer_period;
  
  u16 accel_ramp; //in steps per sec square
  u16 max_speed;  //in steps per sec
  u16 steps_to_break;
  u16 steps_to_do;
}
stepper_motor_t;

void step(stepper_motor_t * m);
void stepper_init(stepper_motor_t * m, pin_t * A1, pin_t * A2, pin_t * B1, pin_t * B2, u8 timer);
//void motor_set_dir(stepper_motor_t * m, u8 dir);
u8 stepper_move(stepper_motor_t * m, u16 steps, u16 max_speed, u8 dir);
#endif