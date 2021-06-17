#include "stepper_logic.h"
#include "stepper_hw.h"

static unsigned long sqrt(unsigned long x)
{
  register unsigned long xr;  // result register
  register unsigned long q2;  // scan-bit register
  register unsigned char f;   // flag (one bit)

  xr = 0;                     // clear result
  q2 = 0x40000000L;           // higest possible result bit
  do
  {
    if((xr + q2) <= x)
    {
      x -= xr + q2;
      f = 1;                  // set flag
    }
    else{
      f = 0;                  // clear flag
    }
    xr >>= 1;
    if(f){
      xr += q2;               // test flag
    }
  } while(q2 >>= 2);          // shift twice
  if(xr < x){
    return xr +1;             // add for rounding
  }
  else{
    return xr;
  }
}

void stepper_isr(stepper_t * m)
{
  unsigned int new_step_delay;        // Holds next delay period.

  m->hw->timer_update (m->step_delay);
  
  switch(m->run_state) {
    case STOP:
      m->step_count = 0;
      m->rest = 0;
      m->hw->timer_stop();
      m->run_state = IDLE;
      break;

    case ACCEL:
      stepper_step(m->hw);
      m->step_count++;
      m->accel_count++;
      new_step_delay = m->step_delay - (((2 * (long)m->step_delay) + m->rest)/(4 * m->accel_count + 1));
      m->rest = ((2 * (long)m->step_delay) + m->rest) % (4 * m->accel_count + 1); // Chech if we should start decelration.
   
      if(m->step_count >= m->decel_start) 
      {
        m->accel_count = m->decel_val;
        m->run_state = DECEL;
      }
      
      else if(new_step_delay <= m->min_delay) // Chech if we hitted max speed.
      {
        m->last_accel_delay = new_step_delay;
        new_step_delay = m->min_delay;
        m->rest = 0;
        m->run_state = RUN;
      }
      break;

    case RUN:
      stepper_step(m->hw);
      m->step_count++;
      new_step_delay = m->min_delay;
      
      if(m->step_count >= m->decel_start) // Chech if we should start decelration.
      {
        m->accel_count = m->decel_val;      
        new_step_delay = m->last_accel_delay; // Start decelration with same delay as accel ended with.
        m->run_state = DECEL;
      }
      break;

    case DECEL:
      stepper_step(m->hw);
      m->step_count++;
      m->accel_count++;
      new_step_delay = m->step_delay - (((2 * (long)m->step_delay) + m->rest)/(4 * m->accel_count + 1));
      m->rest = ((2 * (long)m->step_delay) + m->rest) % (4 * m->accel_count + 1);
      // Check if we at last step
      if(m->accel_count >= 0)
      {
        m->run_state = STOP;
      }
      break;
  }
  m->step_delay = new_step_delay;
}

void stepper_move(stepper_t * m, s16 step)
{
  if(step < 0)
  {
    step = -step;
    m->hw->dir = DIR_CCW;
  }
  else 
  {
    m->hw->dir = DIR_CW;
  }
  m->step_delay = m->step0_delay;
  u32 max_s_lim; //! Number of steps before we hit max speed.
  u32 accel_lim; //! Number of steps before we must start deceleration (if accel does not hit max speed).
  s32 tmp;
  max_s_lim  = m->max_speed;
  max_s_lim *= m->max_speed;
  max_s_lim /= 2 * m->accel;
  
  accel_lim  = step; 
  accel_lim *= m->decel;
  accel_lim /= (m->accel + m->decel);
  
  if(accel_lim <= max_s_lim)
  {
    m->decel_val = accel_lim - step;
  }
  else
  {
    tmp = -max_s_lim;
    tmp *= m->accel;
    tmp /= m->decel;
    m->decel_val = tmp;
  }
  
  m->decel_start = step + m->decel_val;
  m->run_state = ACCEL;
  
  m->accel_count = 0; // Reset counter
  m->hw->timer_update(3);
  m->hw->timer_start();
}


void stepper_set_speed_ramp(stepper_t * m, u16 accel, u16 decel, u16 max_speed)
{
  m->max_speed = max_speed;
  m->accel = accel;
  m->decel = decel;
  m->min_delay = m->timer_frequency_hz / max_speed;
  
  u32 tmp  = m->timer_frequency_hz;
      tmp *= 24;
  u32 z   = accel;
      z  *= 625;
  m->step0_delay = tmp/sqrt(z); //initial step delay 
}