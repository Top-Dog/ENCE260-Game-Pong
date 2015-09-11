/** @file   main_loops.h
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   4 October 2012
 *  @brief  The main loops for each game mode.
 */


#ifndef MAIN_LOOPS_H
#define MAIN_LOOPS_H


#include "game_state.h"


/* Common initialisation */
void main_loops_init(void);

/* Main loops */
void title_main_loop(GameState *state);
void searching_main_loop(GameState *state);
void playing_main_loop(GameState *state);
void waiting_main_loop(GameState *state);
void won_round_main_loop(GameState *state);
void lost_round_main_loop(GameState *state);
void won_game_main_loop(GameState *state);
void lost_game_main_loop(GameState *state);

/** @brief Run the main loop for the given mode */
void main_loop(GameMode mode, GameState *state);


#endif /* MAIN_LOOPS_H */
