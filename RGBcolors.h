#ifndef __RGBCOLORS_H
#define __RGBCOLORS_H

typedef struct
{
  unsigned char R;
  unsigned char G;
  unsigned char B;
}RGBColor_t;


static const RGBColor_t RED            = {255,0,0};
static const RGBColor_t GREEN          = {0,255,0};
static const RGBColor_t BLUE           = {0,0,255};
static const RGBColor_t AZURE          = {0,255,255};
static const RGBColor_t YELLOW         = {255,128,0};
static const RGBColor_t MAGENTA        = {255,0,255};
static const RGBColor_t BLACK          = {0,0,0};
static const RGBColor_t WHITE          = {255,255,255};

RGBColor_t RGB_ColorWheel(unsigned char WheelPos);

#endif