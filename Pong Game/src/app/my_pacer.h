/** @file   my_pacer.h
 *  @author M. P. Hayes, UCECE
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   08 September 2012
 * 
 *  @brief  Paced loop support.
 *
 *  This is a modified version of the included pacer module, which uses a global
 *  variable instead of a function-local static variable to track when the
 *  pacer_wait function should return. This allows a paced loop to be stopped
 *  and started without introducing very long delays like the original module.
 *
 *  Note that, like the original module, only one pacer loop can run at once.
 */


#ifndef PACER_H
#define PACER_H


#include "system.h"


/** @brief Define size of pacer rates.  */
typedef uint16_t pacer_rate_t;


/** @brief Reset the pacer. */
void pacer_clear(void);


/** @brief Wait for the next pacer tick.  */
void pacer_wait(void);


/** @brief Initialise pacer module:
    @param pacer_rate rate in Hz.  */
void pacer_init(pacer_rate_t pacer_rate);


#endif /* PACER_H  */
