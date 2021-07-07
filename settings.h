#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "types.h"
#include "settings_struct.h" // This file should be a part of a user project.
                             // It should containg the definition of a seetings structure.

typedef struct 
{
 settings_struct_t current;
 const settings_struct_t * defaults;
}settings_t;

settings_t settings = 
{
  {0},
  &default_settings
};

void settings_load();
void settings_save();
void settings_reset();

#endif