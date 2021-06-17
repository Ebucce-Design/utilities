#ifndef SPEED_LOGIC_H
#define SPEED_LOGIC_H

#include "types.h"
#include "stepper_hw.h"

#define IDLE   0
#define STOP   1
#define ACCEL  2
#define DECEL  3
#define RUN    4

#define CW  0
#define CCW 1

typedef struct 
{
  u8 run_state;           //! What part of the speed ramp we are in.
  
  u16 max_speed;
  u16 accel;
  u16 decel;
  
  u16 step0_delay;        //! Initial step delay, which defines the acceleration rate
  u16 step_delay;         //! Peroid of next timer delay. At start this value set the accelration rate.
  
  u16 decel_start;        //! What step_pos to start decelaration
  s16 decel_val;          //! Sets deceleration rate.
  u16 min_delay;          //! Minimum time delay (max speed)
  s16 accel_count;        //! Counter used when accelerateing/decelerateing to calculate step_delay
  u32 timer_frequency_hz; //! Frequency of the timer that is used by the motor for impulse generation

  u16 step_count;         // Counting steps when moving.
  u16 rest;               // Keep track of remainder from new_step-delay calculation to increase accurancy
  u16 last_accel_delay;   // Remember the last step delay used when accelrating.
  stepper_hw_desc_t * hw;
} stepper_t;



void step();
void stepper_move(stepper_t * m, s16 step);
void stepper_set_speed_ramp(stepper_t * m, u16 accel, u16 decel, u16 max_speed);
void stepper_isr(stepper_t * m);

void timer_update(u16 c);
void timer_stop(void);
void timer_start(void);

#endif
