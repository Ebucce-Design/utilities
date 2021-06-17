#include "pt_btn_service.h"

PT_THREAD(button_service(button_t * b, struct pt * pt))
{
  PT_BEGIN(pt);
  while(1)
  {
    PT_WAIT_UNTIL(pt, b->get_button_state(b->btn_id) == BUTTON_PRESSED);
    timer_set(&b->t,DEBOUNCE_PERIOD_MS);
    PT_WAIT_UNTIL(pt, timer_expired(&b->t));
    
    if(b->get_button_state(b->btn_id) != BUTTON_PRESSED) continue; //this means it was just bouncing, so no action needed
    
    timer_set(&b->t, LONG_PRESS_DURATION_MS);
    PT_WAIT_UNTIL(pt, timer_expired(&b->t) || b->get_button_state(b->btn_id) != BUTTON_PRESSED);
    
    if(timer_expired(&b->t))                    //Long press detected. Will serve it and then wait until button is released before any further actions
    {
      if(b->long_press_cb) b->long_press_cb(b->btn_id);
      PT_WAIT_UNTIL(pt, b->get_button_state(b->btn_id) != BUTTON_PRESSED);
      continue;
    }
    
    timer_set(&b->t, DOUBLE_CLICK_PAUSE);
    PT_WAIT_UNTIL(pt, timer_expired(&b->t) || b->get_button_state(b->btn_id) == BUTTON_PRESSED);
    
    if(timer_expired(&b->t))                    //Double click NOT detected
    {
      if(b->click_cb) b->click_cb(b->btn_id);
      continue;
    }
    
    if(b->double_click_cb) b->double_click_cb(b->btn_id); // Double click detected
    
    PT_WAIT_UNTIL(pt, b->get_button_state(b->btn_id) == BUTTON_PRESSED);
    }

  PT_END(pt);
}