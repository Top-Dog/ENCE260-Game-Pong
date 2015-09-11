/** @file   game_state.h
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   20 September 2012
 * 
 *  @brief  Structures and functions for managing the game state.
 * 
 *  Note that all co-ordinates are given as (x, y) tuples, where (0, 0)
 *  is the lower left-hand corner and co-ordinates increase upwards
 *  and to the right.
 */


#ifndef GAME_STATE_H
#define GAME_STATE_H


#include "system.h"
#include "vector.h"

/** @brief The different modes the game can be in. */
typedef enum
{
    /** The UCFK has just been turned on, the title screen
     *  is showing, and we're waiting for a game request */
    TITLE,

    /** The button has been pressed and we're searching for a
     *  second player */
    SEARCHING,

    /** The ball is on this UCFK */
    PLAYING,

    /** The ball is on the other UCFK */
    WAITING,

    /** Opponent just missed the ball, showing score */
    WON_ROUND,

    /** Player just missed the ball, showing score */
    LOST_ROUND,

    /** Player just won the game, congratulating */
    WON_GAME,

    /** Player just lost the game, comisserating */
    LOST_GAME
} GameMode;


/** @brief A structure to hold the game score. */
typedef struct {
    /** @brief Player's score. */
    uint8_t player;

    /** @brief Opponent's score. */
    uint8_t opponent;
} GameScore;


/** @brief A structure to hold the game's state. */
typedef struct
{
    /** @brief The ball's current position. 
     * 
     * The x co-ordinate should always be between 0 and 6 inclusive,
     * and the y co-ordinate should always be between 1 and 4 inclusive
     * (assuming it is on this UCFK and not the opponent's). */
    Vector ballPos;
    
    /** @brief The ball's current heading. 
     * 
     * Each co-ordinate should be -1, 0, or 1. */
    Vector ballHeading;
    
    /** @brief The paddle's current position. 
     * 
     * The x co-ordinate of the left-hand side of the paddle. 0 means 
     * the paddle is at the far left, 4 means it is at the far right. */
    uint8_t paddlePos;
    
    /** @brief The current game mode */
    GameMode mode;
    
    /** @brief The current score. */
    GameScore score;

    /** @brief State change timeout. */
    uint16_t timeout;
} GameState;


/** @brief Move the ball one step, handling any collisions, and update the game state. */
void advanceBall(GameState *gameState);


/** @brief Update the paddle position */
void move_paddle(GameState *state, int8_t delta);


#endif /* GAME_STATE_H */
