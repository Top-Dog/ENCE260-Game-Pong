/** @file   my_task.h
 *  @author M. P. Hayes, UCECE
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   7 October 2012
 *  @brief  Modified version of MPH's task scheduler.
 *
 *  This module is a modified version of the original task sceduler from
 *  ucfk4/utils that exits when the game mode changes.
 */


#ifndef TASK_H
#define TASK_H


#include "system.h"
#include "timer.h"

#include "game_state.h"


#define TASK_RATE TIMER_RATE


/** @brief Task function prototype (takes a pointer to the state).  */
typedef void (* task_func_t)(GameState *state);


/** @brief Task structure.  */
typedef struct
{
    /** Function to call.  */
    task_func_t func;
    /** How many ticks between scheduling events.  */
    timer_tick_t period;    
    /** When to reschedule.  */
    timer_tick_t reschedule;
} task_t;


/** Schedule tasks until the game changes state*/
void task_schedule(GameState *state, task_t *tasks, uint8_t num_tasks);


#endif /* TASK_H */
