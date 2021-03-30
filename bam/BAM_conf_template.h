#ifndef __BAM_CONF
#define __BAM_CONF

#include "stm8s.h"

#define BAM_RESOLUTION_BITS             8
#define BAM_LSB_DURATION_TICKS          120

#define REG_A_OFFSET  (0 * BAM_RESOLUTION_BITS)
#define REG_B_OFFSET  (1 * BAM_RESOLUTION_BITS)
#define REG_C_OFFSET  (2 * BAM_RESOLUTION_BITS)
#define REG_D_OFFSET  (3 * BAM_RESOLUTION_BITS)

typedef struct
{
  u8   pin_pos;
  u8   reg_offset;
}
bam_ch_t;

static const bam_ch_t bam_channels[] = 
{
  {3, REG_D_OFFSET}, //channel 1
  {2, REG_D_OFFSET}, //channel 2
  {6, REG_C_OFFSET}, //channel 3
  {5, REG_C_OFFSET}, //channel 4
  {4, REG_B_OFFSET}, //channel 5
  {5, REG_B_OFFSET}, //channel 6
  {3, REG_C_OFFSET}, //channel 7
  {3, REG_A_OFFSET}, //channel 8
  {7, REG_C_OFFSET}, //channel 9
  {1, REG_D_OFFSET}, //channel 10
  {4, REG_D_OFFSET}, //channel 11
  {4, REG_C_OFFSET}  //channel 12
};

/////////////////////////////////////
// these functions should be defined by user
static inline void bam_timer_period_update(u16 period)          
{
  TIM2->ARRH = (uint8_t)(period>> 8); 
  TIM2->ARRL = (uint8_t)(period);
};

static inline void bam_timer_start()
{
  TIM2->CR1 |= (uint8_t)TIM2_CR1_CEN;
}
/////////////////////////////////////

#endif