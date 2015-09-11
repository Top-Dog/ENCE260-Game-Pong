/** @file   main_loop_utils.h
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   4 October 2012
 *  @brief  Helper functions for main loops.
 */


#ifndef MAIN_LOOP_UTILS_H
#define MAIN_LOOP_UTILS_H


#include "timer.h"

#include "main_loops.h"
#include "game_state.h"
#include "bool.h"


/* Task update rates */

/** @brief The frequency the display updates at. */
#define TGL_UPDATE_RATE 500

/** @brief The frequency that tasks that handle updating the display run at. */
#define DISPLAY_TASK_RATE TGL_UPDATE_RATE

/** @brief The frequency tasks that handle player input run at. */
#define INPUT_TASK_RATE 100  /* In Hz */

/** @brief The frequency the task that moves the ball runs at. */
#define BALL_TASK_RATE 5  /* In Hz */

/** @brief The frequency tasks that handle receiving transmissions run at. */
#define RECV_TASK_RATE 250  /* In Hz */

/** @brief The frequency tasks that handle transmitting run at. */
#define SEND_TASK_RATE 10  /* In Hz */


/* Score handling */

/** @brief How long to show the score for. */
#define SCORE_PERIOD 5 /* In seconds */
#define SCORE_TICKS (SCORE_PERIOD * DISPLAY_TASK_RATE)  /* In ticks */

/** @brief How long to show the win/lose message for. */
#define ENDGAME_PERIOD 13.5  /* In seconds */
#define ENDGAME_TICKS (ENDGAME_PERIOD * DISPLAY_TASK_RATE)  /* In ticks */

/** @brief The score required to win the game. */
#define MAX_SCORE 5


/* Minimum time in states */

#define TITLE_MIN (INPUT_TASK_RATE)
/** How many recv ticks to wait before accepting comm input after entering
 *  WAITING. */


/* Function declarations */

/** @brief  Show the current score for SCORE_TICKS ticks, then clear the display */
void show_score(const GameState *state);

/** @brief  Show the given message for ENDGAME_TICKS ticks, then clear the display */
void show_endgame_message(const char *message);

/** @brief  Check whether either player has won, and change the mode appropriately. */
void check_win(GameState *state);

/** @brief  Task which updates the ball and paddle positions and then the display. */
void display_task(GameState *state);

/** @brief  Task which checks the navswitch and moves the paddle appropriately. */
void paddle_task(GameState *state);

/** @brief  Task which moves the ball. */
void ball_task(GameState *state);

/** @brief  Task which checks for ball or score packets and changes mode appropriately. */
void waiting_comm_task(GameState *state);

/** @brief  Task which checks if a game request packet has been received. */
void title_comm_task(GameState *state);

/** @brief  Set the input timeout for the TITLE input task. */
void set_title_timeout(GameState *state);

/** @brief  Task which monitors the button during the title. */
void title_input_task(GameState *state);

/** @brief  Task which updates the display when text is showing. */
void text_display_task(__unused__ GameState *state);

/** @brief  Task which sends game requests. */
void searching_send_task(__unused__ GameState *state);

/** @brief  Task which checks for acknowledgments and starts a game when one is received. */
void searching_recv_task(GameState *state);

/** @brief  Set the score to 0 all. */
void reset_score(GameState *state);

/** @brief  Move the ball into it's initial position. */
void reset_ball(GameState *state);

/** @brief  Move the paddle into it's initial position. */
void reset_paddle(GameState *state);

/** @brief  Move the ball off the display. */
void hide_ball(GameState *state);


#endif /* MAIN_LOOP_UTILS_H */
