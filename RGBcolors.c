#include "RGBcolors.h"

RGBColor_t RGB_ColorWheel(unsigned char WheelPos) 
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
