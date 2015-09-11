/** @file game_state.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   20 September 2012
 */


#include "game_state.h"
#include "debug.h"


/* Define headings */
const Vector DOWN = {0, -1};
const Vector UP = {0, 1};
const Vector DOWN_LEFT = {-1, -1};
const Vector UP_LEFT = {-1, 1};
const Vector DOWN_RIGHT = {1, -1};
const Vector UP_RIGHT = {1, 1};

/* Define positions */
const Vector OFF_BOARD = {-1, -1};


void advanceBall(GameState *state)
{
    const Vector old_pos = state->ballPos;  /* Save some typing */
    Vector new_pos = vector_add(old_pos, state->ballHeading);
    
    /* Ball is going off the bottom of the display: change mode. */
    if (new_pos.y <= -1) {
        state->mode = LOST_ROUND;
    }
    /* Ball is going off the top of the display: change mode. */
    else if(new_pos.y > 4) { 
       state->mode = WAITING; 
    }
    /* Ball is staying on the display. */
    else {
        /* Ball is bouncing off the side of the display: change heading. */
        if(new_pos.x < 0 || new_pos.x > 6) {
            state->ballHeading.x *= -1;
        }
        /* Ball is going to either bounce off the paddle or enter the bottom
         * row: possibly change heading. */
        if (new_pos.y == 0 && state->ballHeading.y == -1) {
            /* Ball is bouncing off center of paddle (same for every heading). */
            if (old_pos.x == state->paddlePos + 1) {
                state->ballHeading.y *= -1;
            }
            /* Ball is going straight down. */
            else if (vector_equal(state->ballHeading, DOWN)) {
                /* Ball is bouncing off left of paddle. */
                if (old_pos.x == state->paddlePos){
                    /* Paddle is in left corner => bounce off to the right. */
                    if (old_pos.x == 0){
                        state->ballHeading = UP_RIGHT; 
                    }
                    /* Bounce off to the left. */
                    else {
                        state->ballHeading = UP_LEFT;
                    }
                }
                /* Ball is bouncing off right of paddle. */
                else if (old_pos.x == state->paddlePos + 2) {
                    /* Paddle is in right corner => bounce off to the left. */
                    if (old_pos.x == 6){
                        state->ballHeading = UP_LEFT; 
                    }
                    /* Bounce off to the right. */
                    else {
                        state->ballHeading = UP_RIGHT;
                    }
                }
            }
            /* Ball is going down and right. */
            else if (vector_equal(state->ballHeading, DOWN_RIGHT)) {
                /* Ball is bouncing off left corner of paddle. */
                if (old_pos.x == state->paddlePos - 1) {
                    if (old_pos.x == 0) {
                        state->ballHeading = UP_RIGHT;
                    }
                    else {
                        state->ballHeading = UP_LEFT;
                    }
                }
                /* Ball is bouncing off left of paddle. */
                else if (old_pos.x == state->paddlePos){
                    state->ballHeading = UP;          
                }
                /* Ball is bouncing off right of paddle. */
                else if (old_pos.x == state->paddlePos + 2) {
                    /* Paddle is in right corner => bounce off to the left. */
                    if (old_pos.x == 6){
                        state->ballHeading = UP_LEFT;
                    }
                    /* Bounce off to the right. */
                    else {
                        state->ballHeading = UP_RIGHT;
                    }
                }
            }
            /* Ball is going down and left. */
            else if (vector_equal(state->ballHeading, DOWN_LEFT)) {
                /* Ball is bouncing off left of paddle. */
                if (old_pos.x == state->paddlePos) {
                    /* Paddle is in left corner => bounce off to the right. */
                    if (state->paddlePos == 0) {
                        state->ballHeading = UP_RIGHT;
                    }
                    /* Bounce off to the left. */
                    else {
                        state->ballHeading = UP_LEFT;
                    }
                }
                /* Ball is bouncing off right of paddle. */
                else if (old_pos.x == state->paddlePos + 2) {
                    state->ballHeading = UP;
                }
                /* Ball is bouncing off right corner of paddle. */
                else if (old_pos.x == state->paddlePos + 3) {
                    /* Paddle is in right corner => bounce off to the left. */
                    if (old_pos.x == 6) {
                        state->ballHeading = UP_LEFT;
                    }
                    /* Bounce off to the right. */
                    else {
                        state->ballHeading = UP_RIGHT;
                    }
                }
            }
        }
        /* Move the ball from the old position according to the (potentially
         * changed) heading. */
        new_pos = vector_add(old_pos, state->ballHeading);
        state->ballPos = new_pos;
    }
}


/*Advances the paddles position based on user input*/
void move_paddle(GameState *state, int8_t delta){
    if (state->paddlePos + delta >= 0 && state->paddlePos + delta <= 4){
        state->paddlePos += delta;
    }
 }
