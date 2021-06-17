#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H
#include "stm8s.h"

// Very minimalistic ringbuffer implementation suitable for small buffers only (less than 256 items)
// Such buffers may come handy when using for UART drivers or something similar

typedef struct
{
  uint8_t *buf;   /** Buffer base address */
  u8 size;        /** Buffer size */
  u8 read_index;  /** Buffer read index */
  u8 write_index; /** Buffer write index */
} ringbuffer_t;



static inline u8 ringbuffer_init(ringbuffer_t * const rb, u8 * buf, u8 size)
{ 
  if ((size & (size - 1)) != 0) // buf size must be aligned to power of 2
  {
    return 1;
  }
  rb->size        = size - 1;  // size - 1 is faster in calculation
  rb->read_index  = 0;
  rb->write_index = rb->read_index;
  rb->buf         = buf;
  return 0;
}

// Get one byte from ringbuffer
static inline u8 ringbuffer_get(ringbuffer_t * const rb, uint8_t * data)
{
  if (rb->write_index != rb->read_index) 
  {
    *data = rb->buf[rb->read_index];
     rb->read_index++;
     rb->read_index &= rb->size;
     return 0;
  }
  return 1;
}

// Put one byte to ringbuffer
static inline void ringbuffer_put(ringbuffer_t * const rb, uint8_t data)
{
  rb->buf[rb->write_index++] = data;
  rb->write_index &= rb->size;
}

// Return the element number of ringbuffer
static inline u8 ringbuffer_get_num(ringbuffer_t * const rb)
{
  return rb->write_index - rb->read_index;
}


// Flush ringbuffer
static inline void ringbuffer_flush(ringbuffer_t * const rb)
{
 rb->read_index = rb->write_index;
}



#endif
