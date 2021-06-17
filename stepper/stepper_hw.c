#include "stepper_hw.h"


static inline void PIN_WriteLow(pin_t * pin)
{
  pin->port->ODR &= (uint8_t)(~pin->pin);
}

static inline void PIN_WriteHigh(pin_t * pin)
{
  pin->port->ODR |= (uint8_t)(pin->pin);
}


void stepper_step(stepper_hw_desc_t * d)
{
  switch (d->phase)
  {
    case 0:{
            PIN_WriteLow   (&d->coil1_B);   
            PIN_WriteLow   (&d->coil2_A);
            PIN_WriteLow   (&d->coil2_B);   
            PIN_WriteHigh  (&d->coil1_A);
            break;
            }
    case 1:{ 
            PIN_WriteLow   (&d->coil1_B);   
            PIN_WriteLow   (&d->coil2_B);       
            PIN_WriteHigh  (&d->coil1_A);
            PIN_WriteHigh  (&d->coil2_A);        
            break;
            }
    case 2:{
            PIN_WriteLow   (&d->coil1_A);
            PIN_WriteLow   (&d->coil1_B);
            PIN_WriteLow   (&d->coil2_B);   
            PIN_WriteHigh  (&d->coil2_A);
            break;
            }
    case 3:{
            PIN_WriteLow   (&d->coil2_B);  
            PIN_WriteLow   (&d->coil1_A);
            PIN_WriteHigh  (&d->coil1_B);  
            PIN_WriteHigh  (&d->coil2_A); 
            break;
            }
    case 4:{
            PIN_WriteLow   (&d->coil2_B);  
            PIN_WriteLow   (&d->coil1_A);
            PIN_WriteLow   (&d->coil2_A);
            PIN_WriteHigh  (&d->coil1_B);  
            break;
          }
    case 5:
          {
            PIN_WriteLow   (&d->coil2_A);  
            PIN_WriteLow   (&d->coil1_A);
            PIN_WriteHigh  (&d->coil2_B);
            PIN_WriteHigh  (&d->coil1_B);  
            break;
          }
    case 6:
          {
            PIN_WriteLow   (&d->coil2_A);  
            PIN_WriteLow   (&d->coil1_A);
            PIN_WriteLow   (&d->coil1_B);  
            PIN_WriteHigh  (&d->coil2_B);
            break;
          }
    case 7:
          {
            PIN_WriteLow    (&d->coil2_A);  
            PIN_WriteLow    (&d->coil1_B);
            PIN_WriteHigh   (&d->coil1_A);
            PIN_WriteHigh   (&d->coil2_B);     
            break;
          }
  }

  if(d->dir == DIR_CCW) 
  {
    d->phase += d->phase_step;
    if(d->phase >= 8) {d->phase = 0;} 
  }
  
  if(d->dir == DIR_CW) 
  {
    d->phase -= d->phase_step;
    if(d->phase < 0) {d->phase = 7;} 
  }
}

void stepper_off(stepper_hw_desc_t * d)
{
  PIN_WriteLow (&d->coil1_A);
  PIN_WriteLow (&d->coil1_B);
  PIN_WriteLow (&d->coil2_A);
  PIN_WriteLow (&d->coil2_B);
}