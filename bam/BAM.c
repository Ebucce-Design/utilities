#include "BAM.h"

static u8  update_flag          = 0;
static u16 lsb_period_ticks     = 0;
static u8* active_buffer        = {0};
static u8  step                 = 0;
static u16 period               = 0;
static u8 reverse_flag          = 0;
static u8 buf1[BAM_RESOLUTION_BITS * BAM_GPIO_PORTS_USED_NUM]; // 4 here is caused by the fact that BAM uses 4 GPIO different ports
static u8 buf2[BAM_RESOLUTION_BITS * BAM_GPIO_PORTS_USED_NUM];

static u8 bitmask_A = 0;
static u8 bitmask_B = 0;
static u8 bitmask_C = 0;
static u8 bitmask_D = 0;

static u8 bam_values[BAM_CHANNELS_NUM]; //this array is only used to be able to read channel values back 

void BAM_isr()  
{ 
  if(reverse_flag == 0)
  {
    if(step == BAM_RESOLUTION_BITS - 1) 
    {
      reverse_flag = 1;
      if(update_flag)
      {
        active_buffer = (active_buffer == buf1) ? buf2 : buf1;
        update_flag = 0;
      }
    }
    else
    {
      step++;
      period  <<= 1;
    }
  }
  else
  {
    if(step == 0) 
    {   
      reverse_flag = 0; 
      if(update_flag)
      {
        active_buffer = (active_buffer == buf1) ? buf2 : buf1;
        update_flag = 0;
      }
    }
    else
    {
      step--;
      period  >>= 1;
    }
  }

  bam_timer_period_update(period);
  GPIOA->ODR &= ~bitmask_A;
  GPIOA->ODR |= active_buffer[REG_A_OFFSET + step];
  GPIOB->ODR &= ~bitmask_B;
  GPIOB->ODR |= active_buffer[REG_B_OFFSET + step];
  GPIOC->ODR &= ~bitmask_C;
  GPIOC->ODR |= active_buffer[REG_C_OFFSET + step];
  GPIOD->ODR &= ~bitmask_D;
  GPIOD->ODR |= active_buffer[REG_D_OFFSET + step];
}


u8 bam_set_channel(u8 channel, u8 value)
{
  if(channel > ARRAY_SIZE(bam_channels)) return 1;
  update_flag = 0; //to avoid using not prepared propery data
  u8 bitmask = 1;
  const bam_ch_t * ch = &bam_channels[channel];  
  u8 * array = (active_buffer == buf1) ? buf2 : buf1;
  array += ch->reg_offset;
  for(u8 i = 0; i < BAM_RESOLUTION_BITS; i++)
    {
    if(value & bitmask)
      {
      array[i] |= 1 << ch->pin_pos;  
      }
      else
      {
      array[i] &= ~(1 << ch->pin_pos);  
      }
      bitmask <<= 1;
    }
  bam_values[channel] = value;
  return 0;
}

void bam_update()
{
  update_flag = 1;
}

u8 update_gpio_bitmask(u8 offset)
{
  u8 bitmask = 0;
  for(u8 i = 0; i < ARRAY_SIZE(bam_channels); i++)
  {
    if(bam_channels[i].reg_offset == offset)
    {
      bitmask |= 1 << bam_channels[i].pin_pos;
    }
  }
  return bitmask;
}


void bam_init()
{
  bitmask_A = update_gpio_bitmask(REG_A_OFFSET);
  bitmask_B = update_gpio_bitmask(REG_B_OFFSET);
  bitmask_C = update_gpio_bitmask(REG_C_OFFSET);
  bitmask_D = update_gpio_bitmask(REG_D_OFFSET);
  active_buffer = buf1;
  lsb_period_ticks = BAM_LSB_DURATION_TICKS;
  period = lsb_period_ticks;
  bam_timer_period_update(period);
  bam_timer_start();
}

u8 bam_get_channel(u8 channel)
{
  if(channel > ARRAY_SIZE(bam_channels)) return 1;
  return bam_values[channel];
}