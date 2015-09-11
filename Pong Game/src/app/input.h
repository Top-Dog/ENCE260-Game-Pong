/** @file   input.h
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   28 September 2012
 *  @brief  Game-specific methods for checking for user input
 */


#ifndef INPUT_H 
#define INPUT_H


#include "game_state.h"


/** @brief Initialise the module. */
void input_init(void);


/** @brief Move the paddle according to the input. */
void input_do_paddle(GameState *state);


#endif /* INPUT_H */
