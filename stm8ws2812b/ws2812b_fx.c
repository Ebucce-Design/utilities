#include "ws2812b_fx.h"
#include "pt.h"

#include <stdlib.h>



const RGBColor_t * pure_colors[] = {&RED, &GREEN, &BLUE, &YELLOW, &MAGENTA, &WHITE};


RGBColor_t get_next_color()
{
static u8 index = 0;
RGBColor_t t = *pure_colors[index++];
if (index == ARRAY_LENGTH(pure_colors)) index = 0;
return t;
}

RGBColor_t Wheel(unsigned char WheelPos) 
{
  WheelPos = 255 - WheelPos;
  RGBColor_t color;
  if(WheelPos < 85) 
  {
    color.R = 255 - WheelPos * 3;
    color.G = 0;
    color.B = WheelPos * 3;
  return color;
  }
  if(WheelPos < 170) 
  {
    WheelPos -= 85;
    color.R = 0;
    color.G = WheelPos * 3;
    color.B = 255 - WheelPos * 3;
  return color;
  }
  
  WheelPos -= 170;
  color.R = WheelPos * 3; 
  color.G = 255 - WheelPos * 3;
  color.B = 0;
  return color;
}



void set_brightness(RGBColor_t *color,u8 br)
{
  u16 z = 0;
  z = (color->B*br)>>8;
  color->B = z;
  z = (color->R*br)>>8;
  color->R = z;
  z = (color->G*br)>>8;
  color->G = z;
}