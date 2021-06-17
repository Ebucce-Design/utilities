#ifndef __PT_BUTTON_SERVICE
#define __PT_BUTTON_SERVICE

#include "time_service.h"
#include "pt.h"

#define LONG_PRESS_DURATION_MS  2000
#define DEBOUNCE_PERIOD_MS      0
#define DOUBLE_CLICK_PAUSE      250

#define BUTTON_PRESSED  1
#define BUTTON_RELEASED 0

typedef struct
{
    char btn_id;
    timer_t t;
    void (*click_cb)(char btn_id);             //single click callback
    void (*double_click_cb)(char btn_id);      //double click callback
    void (*long_press_cb)(char btn_id);        //long press callback
    char (*get_button_state)(char btn_id);     //function that reads the pin state. 
                                                //Returns 1 if button pressed, else - 0
}button_t;

PT_THREAD(button_service(button_t * b, struct pt * pt));

#endif