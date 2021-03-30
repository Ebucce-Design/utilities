#ifndef __BAM
#define __BAM

#include "BAM_conf.h"

#define ARRAY_SIZE(arr)                 (sizeof(arr) / sizeof(arr[0]))


void bam_set_channel(u8 channel, u8 value);
void bam_init();
void bam_update();
#endif