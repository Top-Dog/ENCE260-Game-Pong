/** @file   my_pacer.c
 *  @author M. P. Hayes, UCECE
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   08 September 2012
 */


#include "system.h"
#include "timer.h"

#include "my_pacer.h"

/** @brief Number of timer ticks to wait. */
static timer_tick_t pacer_period;

/** @brief Next timer value to wait for. */
static timer_tick_t when = 0;


void pacer_init (pacer_rate_t pacer_rate) {
    timer_init ();
    pacer_period = TIMER_RATE / pacer_rate;
}

void pacer_clear(void) {
    when = timer_get() + pacer_period;
}

void pacer_wait (void) {
    timer_wait_until (when);
    when += pacer_period;
}
