#ifndef __TIME_SERVICE
#define __TIME_SERVICE

#include "stm8s.h"

#define FREQ                    16000000
#define TIMER_PRESCALER         128
#define TIMER_PERIOD            125

typedef u32 timer_t;

void    timer_set(timer_t * t, u32 interval_ms);
u8      timer_expired(timer_t* t);
void    delay_blocking_ms(u16 delay);
void    time_service_timer_init();
#endif