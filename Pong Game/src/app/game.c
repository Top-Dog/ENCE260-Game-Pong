/** @file   game.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   25 September 2012
 * 
 *  @brief  The main loop for the game.
 *
 *  The game is modeled as a state machine with six 'modes':
 *  - TITLE (initial state): waiting for a game request. Transitions to SEARCHING
 *    when the button is pressed, or to WAITING when a game request is received.
 *  - SEARCHING: Sending game requests. Transitions to PLAYING when an
 *    acknowledgment is received.
 *  - PLAYING: The ball is on this UCFK. Transitions to LOST_ROUND if the player
 *    misses the ball, or to WAITING when the ball goes onto the other UCFK.
 *  - WAITING: The ball is on the other UCFK. Transitions to PLAYING when the
 *    ball comes back on to this UCFK, or to WON_ROUND when the opponent misses
 *    the ball.
 *  - WON_ROUND: The opponent missed the ball. Transitions to WAITING after a
 *    set time.
 *  - LOST_ROUND: The player missed the ball. Transitions to PLAYING after a set
 *    time.
 *  - WON_GAME: The player won the game. Transitions to TITLE after a set time.
 *  - LOST_GAME: The player lost the game. Transitions to TITLE after a set time.
 *
 *  Each mode has its own main loop. To signal a transition, the main loops
 *  simply return. The game main loop (which is all that is in this module)
 *  repeatedly calls the main loop for the current mode.
 */


#include "game_state.h"
#include "main_loops.h"
#include "main_loop_utils.h"


int main(void) {	
    GameState state = 
    {
        .mode = TITLE,
    };

    reset_ball(&state);
    reset_paddle(&state);
    reset_score(&state);
    
    main_loops_init();

    while (1) {
		main_loop(state.mode, &state);
    }
}
