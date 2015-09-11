/** @file   input.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   28 September 2012
 */


#include "system.h"
#include "navswitch.h"

#include "input.h"
#include "game_state.h"


/** @brief How many input ticks to wait before repeating a paddle movement. */
#define WAIT_TICKS 15


void input_init(void){
    navswitch_init();
}


void input_do_paddle(GameState *state){
    static uint8_t WaitCount = 0;
    
    navswitch_update();

    /* Navswitch just pressed. */
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        /* Move to the right */
        move_paddle(state, 1);
        WaitCount = 0;
    }
    if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        /* Move to the left */
        move_paddle(state, -1);
        WaitCount = 0;
    }

    /* Navswitch pressed more than WAIT_TICKS ticks ago. */
    if (navswitch_down_p(NAVSWITCH_NORTH) && WaitCount > WAIT_TICKS) {
        /* Move to the right */
        move_paddle(state, 1);
        WaitCount = 0;
    }
    if (navswitch_down_p(NAVSWITCH_SOUTH) && WaitCount > WAIT_TICKS) {
        /* Move to the left */
        move_paddle(state, -1);
        WaitCount = 0;
    }
    
    WaitCount += 1;
}
