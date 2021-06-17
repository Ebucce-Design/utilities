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



static const unsigned char gamma_table[] = 
{
  0,  0,  0,  0,  0,  1,  1,  2,
  2,  3,  4,  5,  6,  7,  8,  9,
 11, 13, 14, 16, 18, 20, 23, 25,
 28, 30, 33, 36, 39, 43, 46, 50,
 54, 58, 62, 66, 70, 75, 80, 85,
 90, 95,100,106,112,118,124,130,
136,143,150,157,164,171,179,187,
194,203,211,219,228,237,246,255
};

RGBColor_t set_GC_brightness (unsigned char br, RGBColor_t color)
{
  if(br > ARRAY_SIZE(gamma_table)) br = GC_MAX_BRIGHTNESS;
  unsigned short tmp = 0;
  tmp = gamma_table[br] * color.R;
  tmp >>= 8;
  color.R = tmp;

  tmp = gamma_table[br] * color.G;
  tmp >>= 8;
  color.G = tmp;

  tmp = gamma_table[br] * color.B;
  tmp >>= 8;
  color.B = tmp;
  return color;
}