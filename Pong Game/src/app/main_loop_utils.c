/** @file   main_loop_utils.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   4 October 2012
 */


#include <stdio.h>
#include "tinygl.h"
#include "timer.h"
#include "system.h"
#include "pio.h"
#include "button.h"

#include "main_loop_utils.h"
#include "debug.h"
#include "main_loops.h"
#include "game_state.h"
#include "bool.h"
#include "comm.h"
#include "draw.h"
#include "input.h"
#include "my_pacer.h"


/** @brief  Update tinygl for a given number of timer ticks. */
static void update_for(timer_tick_t ticks) {
    timer_tick_t i = 0;
    pacer_clear();
    /* Keep updating for the given number of ticks, and make sure that comm
     * finishes sending (just in case). */
    while (comm_is_sending() || i < ticks) {
        i += 1;
        tinygl_update();
        comm_attempt_send();
        pacer_wait();
    }
}


void show_score(const GameState *state) {
    char score[3];
    TRACE("In show_score.\n");
    snprintf(score, 3, "%i%i", 
             state->score.player, state->score.opponent);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
    tinygl_text(score);
    update_for(SCORE_TICKS);
}


void show_endgame_message(const char *message) {
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(message);
    update_for(ENDGAME_TICKS);
}


/** @brief  Check whether the player has won. */
static bool player_won(const GameState *state) {
    return state->score.player >= MAX_SCORE;
}

/** @brief  Check whether the opponent has won. */
static bool opponent_won(const GameState *state) {
    return state->score.opponent >= MAX_SCORE;
}

void check_win(GameState *state) {
    if (player_won(state)) {
        state->mode = WON_GAME;
    }
    if (opponent_won(state)) {
        state->mode = LOST_GAME;
    }
}


void display_task(GameState *state) {
    draw_clear();
    draw_ball(state);
    draw_paddle(state);
    draw_update();
}


void paddle_task(GameState *state) {
    input_do_paddle(state);
}


void ball_task(GameState *state) {
    advanceBall(state);
}


void waiting_comm_task(GameState *state) {
    if (comm_is_sending()) {
        /* Haven't finished transmission from the last state */
        comm_attempt_send();
    }
    else {
        //pio_config_set(LED1_PIO, PIO_OUTPUT_LOW);
        /* Process received packets */
        comm_attempt_recv();
        if (comm_is_packet_ready(BALL_PACKET)) {
            comm_recv_ball(state);
            comm_clear_packet();
            state->mode = PLAYING;
        }
        if (comm_is_packet_ready(SCORE_PACKET)) {
            comm_recv_score(&state->score);
            comm_clear_packet();
            state->mode = WON_ROUND;
            check_win(state);
        }
    }
}


/** @brief  Update the display until the currently sending packet has sent. */
static void send_while_drawing(void) {
    pacer_clear();
    while (comm_is_sending()) {
        comm_attempt_send();
        draw_update();
        pacer_wait();
    }
}


void title_comm_task(GameState *state) {
    comm_attempt_recv();
    if (comm_is_packet_ready(GAME_REQUEST_PACKET)) {
        state->mode = WAITING;
        tinygl_clear();
        comm_send_ack();
        send_while_drawing();
    }
}


void set_title_timeout(GameState *state) {
    state->timeout = TITLE_MIN;
}


void title_input_task(GameState *state) {
    button_update();
    if (state->timeout > 0) {
        state->timeout -= 1;
    }
    else if (button_push_event_p(0)) {
        state->mode = SEARCHING;
        tinygl_clear();
        //actually_clear_display();
    }
}


void text_display_task(__unused__ GameState *state) {
    draw_update();
}


void searching_send_task(__unused__ GameState *state) {
    /* Send game request */
    if (comm_is_sending()) {
        comm_attempt_send();
    }
    else {
        comm_send_game_request();
    }
}


void searching_recv_task(GameState *state) {
    /* Check for acknowledgement */
    comm_attempt_recv();
    if (comm_is_packet_ready(ACK_PACKET)) {
        /* Start new game */
        state->mode = PLAYING;
        reset_ball(state);
    }
}


void reset_score(GameState *state) {
    state->score.player = 0;
    state->score.opponent = 0;
}


void reset_ball(GameState *state) {
    state->ballPos = vector_create(3, 5); /* Initially off screen */
    state->ballHeading = vector_create(0, -1);
}


void reset_paddle(GameState *state) {
    state->paddlePos = 2;  /* Center */
}


void hide_ball(GameState *state) {
    state->ballPos = vector_create(-1, -1);
}
