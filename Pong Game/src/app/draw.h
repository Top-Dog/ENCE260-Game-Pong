/** @file   draw.h
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   6 October 2012
 *  @brief  TinyGL based game-specific drawing routines.
 */


#ifndef DRAW_H
#define DRAW_H


#include "game_state.h"


/** @brief Initialise the module. */
void draw_init(uint16_t rate);


/** @brief Clear the display */
void draw_clear(void);


/** @brief Draw the ball */
void draw_ball(const GameState *state);


/** @brief Draw the paddle */
void draw_paddle(const GameState *state);


/** @brief Update the display */
void draw_update(void);


#endif /* DRAW_H */
