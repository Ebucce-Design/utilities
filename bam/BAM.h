#ifndef __BAM
#define __BAM

#include "BAM_conf.h"

u8      bam_set_channel(u8 channel, u8 value);
u8      bam_get_channel(u8 channel);
void    bam_init();
void    bam_update();
#endif