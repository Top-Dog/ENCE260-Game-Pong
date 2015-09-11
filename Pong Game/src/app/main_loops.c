/** @file main_loops.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   4 October 2012
 */


#include <stdio.h>
#include "system.h"
#include "pio.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "button.h"

#include "main_loops.h"
#include "game_state.h"
#include "debug.h"
#include "main_loop_utils.h"
#include "comm.h"
#include "draw.h"
#include "my_task.h"
#include "my_pacer.h"
#include "input.h"


/** @brief Speed at which to scroll text. */
#define TGL_TEXT_SPEED 20

/** @brief Font to display text with. */
#define TGL_FONT (&font3x5_1)


/** @brief Type of pointer to main loop function */
typedef void (*main_loop_t)(GameState*);


/** @brief List of main loops. */
const main_loop_t mainLoops[] =
{
    title_main_loop,
    searching_main_loop,
    playing_main_loop,
    waiting_main_loop,
    won_round_main_loop,
    lost_round_main_loop,
    won_game_main_loop,
    lost_game_main_loop
};


void main_loops_init(void) {
    system_init();
    debug_init();
    input_init();
    button_init();
    comm_init();
    draw_init(TGL_UPDATE_RATE);
    pacer_init(DISPLAY_TASK_RATE);

    tinygl_font_set (TGL_FONT);
    tinygl_text_speed_set (TGL_TEXT_SPEED);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    
    TRACE("\n\n\n**UCFK initialised.**\n");
}


void title_main_loop(GameState *state) {
    /* The initial mode: displaying the title screen and waiting for a
     * game request packet. If a game request packet is received,
     * change mode to WAITING. If the button is pressed, change state
     * to SEARCHING.
     */

    task_t tasks[] =
    {
        {.func = text_display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = title_input_task, .period = TASK_RATE / INPUT_TASK_RATE},
        {.func = title_comm_task, .period = TASK_RATE / RECV_TASK_RATE},
    };
        
    TRACE("\nIn title main loop.\n");
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("  PONG");

    set_title_timeout(state);
    task_schedule(state, tasks, 3);
}


void searching_main_loop(GameState *state) {
    /* The button has been pressed on the title screen. This UCFK will
     * change mode to PLAYING when it finds another UCFK in the TITLE
     * state.
     */

    task_t tasks[] =
    {
        {.func = text_display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = searching_send_task, .period = TASK_RATE / SEND_TASK_RATE},
        {.func = searching_recv_task, .period = TASK_RATE / RECV_TASK_RATE},
    };
    
    TRACE("\nIn searching main loop.\n");
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("  SEARCHING");
    
    task_schedule(state, tasks, 3);
}


void playing_main_loop(GameState *state) {
    /* As in singleplayer. */
    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = paddle_task, .period = TASK_RATE / INPUT_TASK_RATE},
        {.func = ball_task, .period = TASK_RATE / BALL_TASK_RATE},
    };
    
    TRACE("\nIn playing main loop.\n");

    task_schedule(state, tasks, 3);

    ASSERT(state->mode == LOST_ROUND || state->mode == WAITING);
    if (state->mode == LOST_ROUND) {
        state->score.opponent += 1;
        comm_send_score(state->score);
        /* update_for sends this packet. */
        check_win(state);
        tinygl_clear();
        /* send_while_drawing(); */
        /* actually_clear_display(); */
    }
    if (state->mode == WAITING) {
        comm_send_ball(state);
        /* Waiting comm task sends this packet */
        hide_ball(state);
        display_task(state);
    }
}


void waiting_main_loop(GameState *state) {
    /* Paddle is showing and movable but ball is on other UCFK. Waiting
     * for game state packet.
     */
    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = paddle_task, .period = TASK_RATE / INPUT_TASK_RATE},
        {.func = waiting_comm_task, .period = TASK_RATE / RECV_TASK_RATE},
    };
    TRACE("\nIn waiting main loop.\n");
    task_schedule(state, tasks, 3);
    
    /* actually_clear_display(); */
}


void won_round_main_loop(GameState *state) {
    /* Opponent just missed the ball, displaying score. Change mode to
     * WAITING after a certain amount of time.
     */
    TRACE("\nIn won round main loop.\n");
    /* Just receieved score from opponent, so do not update score */
    show_score(state);
    state->mode = WAITING;
    reset_ball(state);
    /* actually_clear_display(); */
}


void lost_round_main_loop(GameState *state) {
    /* Player just missed the ball, displaying score. Change mode to 
     * PLAYING after a certain amount of time.
     */
    TRACE("\nIn lost round main loop.\n");
    show_score(state);
    state->mode = PLAYING;
    reset_ball(state);
    /* actually_clear_display(); */
    
}


void won_game_main_loop(GameState *state) {
    /* Showing winner's message, change mode to TITLE after a certain
     * amount of time.
     */
    TRACE("\nIn won game main loop.\n");
    show_endgame_message("  YOU WIN ");
    state->mode = TITLE;
    reset_score(state);
    reset_paddle(state);
    hide_ball(state);
}


void lost_game_main_loop(GameState *state) {
    /* Showing loser's message, change mode to TITLE after a certain 
     * amount of time.
     */
    TRACE("\nIn lost game main loop.\n");
    show_endgame_message("  YOU LOSE");
    state->mode = TITLE;
    reset_score(state);
    reset_paddle(state);
    hide_ball(state);
}


void main_loop(GameMode mode, GameState *state) {
    mainLoops[mode](state);
}
