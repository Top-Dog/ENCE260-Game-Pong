/** @file   draw.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   6 October 2012
 */


#include "tinygl.h"

#include "draw.h"
#include "game_state.h"


/** @brief Convert a point from board coords to tinygl coords */
static tinygl_point_t to_tgl_coords(int8_t x, int8_t y) {
    tinygl_point_t point = {4 - y, 6 - x};
    return point;
}


void draw_init(uint16_t rate) {
    tinygl_init(rate);
}


void draw_clear(void) {
    tinygl_clear();
}


void draw_ball(const GameState *state) {
    if (state->ballPos.y >= 0 && state->ballPos.y <= 4) { //this is so that the ball only prints if its on the screen
        tinygl_point_t point = to_tgl_coords(state->ballPos.x, state->ballPos.y);
        tinygl_draw_point(point, 1);
    }
}


void draw_paddle(const GameState *state) {
    tinygl_point_t start = to_tgl_coords(state->paddlePos, 0);
    tinygl_point_t end = to_tgl_coords(state->paddlePos + 2, 0);
    tinygl_draw_line(start, end, 1);
}


void draw_update(void) {
    tinygl_update();
}
