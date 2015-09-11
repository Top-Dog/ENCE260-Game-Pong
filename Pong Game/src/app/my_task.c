/** @file   my_task.h
 *  @author M. P. Hayes, UCECE
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   7 October 2012
 */


#include "system.h"
#include "timer.h"

#include "my_task.h"
#include "game_state.h"


/** With 16-bit times the maximum value is 32768.  */
#define TASK_OVERRUN_MAX 32767


void task_schedule(GameState *state, task_t *tasks, uint8_t num_tasks) {
    uint8_t i;
    timer_tick_t now;
    task_t *next_task;
    GameMode initial_mode = state->mode;

    timer_init();
    now = timer_get();

    /* Start by scheduling the first task.  */
    next_task = tasks;

    while (state->mode == initial_mode) {
        timer_tick_t sleep_min;

        /* Wait until the next task is ready to run.  */
        timer_wait_until (next_task->reschedule);

        /* Schedule the task.  */
        next_task->func (state);

        /* Update the reschedule time.  */
        next_task->reschedule += next_task->period;

        sleep_min = ~0;
        now = timer_get ();

        /* Search array of tasks.  Schedule the first task (highest priority)
           that needs to run otherwise wait until first task ready.  */
        for (i = 0; i < num_tasks; i++) {
            task_t * task = tasks + i;
            timer_tick_t overrun;

            overrun = now - task->reschedule;
            if (overrun < TASK_OVERRUN_MAX) {
                /* Have found a task that can run immediately.  */
                next_task = task;
                break;
            }
            else {
                timer_tick_t sleep;

                sleep = -overrun;
                if (sleep < sleep_min)
                {
                    sleep_min = sleep;
                    next_task = task;
                }
            }
        }
    }
}
