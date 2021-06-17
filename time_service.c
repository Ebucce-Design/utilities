#include "time_service.h"

//static u16 tick_freq =  FREQ / (TIMER_PRESCALER *  TIMER_PERIOD);
volatile u32 uptime = 0;

void uptime_routine(void)
{
if(uptime++ == 0xFFFFFFFF) { uptime  = 0; }
}

u32 get_uptime()
{
  TIM4_ITConfig    (TIM4_IT_UPDATE, DISABLE);
  u32 time = uptime;
  TIM4_ITConfig    (TIM4_IT_UPDATE, ENABLE);
  return time;
}

inline void timer_set(timer_t * t, u32 interval)
{
  *t = get_uptime() + interval;
}

inline u8 timer_expired(timer_t * t)
{
  if (get_uptime() > *t) 
  {
    return 1;
  }
    return 0;
}

void delay_blocking_ms(u16 delay)
{
u32 expire_time = get_uptime() + delay;

while(expire_time > get_uptime())
{
  asm("NOP");
} 
}

void time_service_timer_init()
{
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIMER_PERIOD);
  TIM4_ITConfig    (TIM4_IT_UPDATE, ENABLE);
  TIM4_ClearFlag   (TIM4_FLAG_UPDATE);
  TIM4_Cmd         (ENABLE);
}